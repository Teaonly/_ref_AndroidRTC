#include <assert.h>
#include "helper.h"
#include "rtpstreamer.h"
#include "mediachannel.h"
#include "h264encoder.h"

enum {
    MSG_START_CONNECT,
    MSG_CHECK_CONNECT,
};

RtpStreamer::RtpStreamer(talk_base::Thread* streaming_thread, talk_base::Thread* encoding_thread) {
    state_ = STATE_IDLE;
    channel_ = NULL;
    encoder_ = NULL;
    streaming_thread_ = streaming_thread;
    encoding_thread_ = encoding_thread;
}

RtpStreamer::~RtpStreamer() {
    StopStreaming();
}

void RtpStreamer::OnMessage(talk_base::Message *msg) {
    if(msg->message_id == MSG_START_CONNECT ) {
        doConnect();
    }
}

int RtpStreamer::StartStreaming(const std::string& url, const std::string& description) {
    if ( url_.Parse(url) == false)
        return -1;

    if ( description_.Parse(description) == false)
        return -1;
    
    if ( description_.isAudio ) {
        // We don't support current
        return -1;
    }
     
    //create encoder and channel object
    channel_ = CreateChannel(url_.proto, NULL);
    if ( channel_ == NULL) {
        return -1;
    }
    channel_->SignalChannelOpened.connect(this, &RtpStreamer::OnChannelOpened);
    channel_->SignalChannelClosed.connect(this, &RtpStreamer::OnChannelClosed);
    channel_->SignalDataRead.connect(this, &RtpStreamer::OnChannelDataRead);

    encoder_ = new H264Encoder(encoding_thread_);
    encoder_->SignalCodedBuffer.connect(this, &RtpStreamer::OnCodedBuffer);
    
    if ( encoder_->Prepare(description_) < 0) {
        delete encoder_;
        encoder_ = NULL;
        return -1;
    }
      
    streaming_thread_ ->Post(this, MSG_START_CONNECT);
    state_ = STATE_IDLE;
    return 0;
}

int RtpStreamer::StopStreaming() {
    if ( encoder_ != NULL) {
        encoder_->Release();
        delete encoder_;
        encoder_ = NULL;
    }
    if ( channel_ != NULL) {
        channel_->Close();
        delete channel_;
        channel_ = NULL;
    }
    return 0;
}

void RtpStreamer::doConnect() {
    assert(channel_ != NULL);
    
    channel_->Connect(url_);
}

int RtpStreamer::ProvideCameraFrame(unsigned char *yuvData) {
    
    LOGD("  ProvideCameraFrame, state_ = %d",  state_);
    // in the calling thread
    if ( state_ != STATE_STREAMING) {
        return -1;
    }
    
    encoder_->EncodePicture(yuvData);
    return 0;
}


void RtpStreamer::OnCodedBuffer(H264Encoder* enc, const unsigned char* codedBuffer, const unsigned int& length) {
    // in encoding thread 
    
}

void RtpStreamer::OnChannelOpened(MediaChannel *ch, const bool& isOK) {
    // in streaming thread
    if ( isOK ) {
        state_ = STATE_STREAMING;
    }
    LOGD("_______________________________OnChannelOpened = %d-------------, state_ = %d", isOK, state_);
    SignalStreamingBegin(this, isOK);
}

void RtpStreamer::OnChannelClosed(MediaChannel *ch) {
    // in streaming thread
    state_ = STATE_IDLE;
    if ( encoder_ != NULL) {
        encoder_->Release();
    }  

    SignalStreamingEnd(this);
}

void RtpStreamer::OnChannelDataRead(MediaChannel *ch, const unsigned char* data, const unsigned int& length) {
    // handling RTCP package
}

