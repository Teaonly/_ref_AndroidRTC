#include "helper.h"
#include "rtpstreamer.h"
#include "mediachannel.h"
#include "h264encoder.h"

enum {
    MSG_START_CONNECT,
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
    encoder_ = new H264Encoder(encoding_thread_);
    encoder_->SignalCodedBuffer.connect(this, &RtpStreamer::OnCodedBuffer);
    if ( encoder_->Prepare(description_.width, description_.height) < 0) {
        delete encoder_;
        encoder_ = NULL;
        return -1;
    }

    channel_ = CreateChannel(url_.proto, NULL);
    channel_->SignalChannelOpened.connect(this, &RtpStreamer::OnChannelOpened);
    channel_->SignalChannelClosed.connect(this, &RtpStreamer::OnChannelClosed);
    channel_->SignalDataRead.connect(this, &RtpStreamer::OnChannelDataRead);
    
    streaming_thread_ ->Post(this, MSG_START_CONNECT);

    return 0;
}

int RtpStreamer::ProvideCameraFrame(unsigned char *yuvData, int wid, int hei) {
    if ( state_ == STATE_STREAMING) {
        return 0;
    }    

    return 1;
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
    
}

void RtpStreamer::OnCodedBuffer(H264Encoder* enc, const unsigned char* codedBuffer, const unsigned int& length) {

}

void RtpStreamer::OnChannelOpened(MediaChannel *ch, const bool& isOK) {

}

void RtpStreamer::OnChannelClosed(MediaChannel *ch) {

}

void RtpStreamer::OnChannelDataRead(MediaChannel *ch, const unsigned char* data, const unsigned int& length) {

}


