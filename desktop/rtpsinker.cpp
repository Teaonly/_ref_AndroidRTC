#include <assert.h>
#include "media.h"
#include "rtpsinker.h"
#include "datachannel.h"
#include "mediabuffer.h"
#include "talk/session/phone/rtputils.h"

enum {
    MSG_START_LISTEN,
};

const int kMaxRTPSize = 1350;
const int kRTPHeaderSize = 12;
const int kMaxPayloadSize = kMaxRTPSize - kRTPHeaderSize;

RtpSinker::RtpSinker(talk_base::Thread* streaming_thread) {
    state_ = STATE_IDLE;
    channel_ = NULL;
    streaming_thread_ = streaming_thread;
    
    buffer_ = new MediaBuffer(64, kMaxRTPSize+128); 
}

RtpSinker::~RtpSinker() {
    StopSinking();
    streaming_thread_->Quit();
    delete buffer_;
}

void RtpSinker::OnMessage(talk_base::Message *msg) {
    if(msg->message_id == MSG_START_LISTEN ) {
        doListen();
    }
}

int RtpSinker::StartSinking(const std::string& url, const std::string& description, const unsigned int& ssrc) {
    if ( url_.Parse(url) == false)
        return -1;
    if ( description_.Parse(description) == false)
        return -1;
    if ( description_.isAudio ) {
        // We don't support current
        return -1;
    }
    rtpSSRC_ = ssrc;

    //create encoder and channel object
    channel_ = CreateChannel(url_.proto, NULL);
    if ( channel_ == NULL) {
        return -1;
    }
    channel_->SignalChannelOpened.connect(this, &RtpSinker::OnChannelOpened);
    channel_->SignalChannelClosed.connect(this, &RtpSinker::OnChannelClosed);
    channel_->SignalDataRead.connect(this, &RtpSinker::OnChannelDataRead);

    buffer_->Reset();
    state_ = STATE_IDLE;
    streaming_thread_ ->Post(this, MSG_START_LISTEN);

    return 0;
}

int RtpSinker::StopSinking() {
    if ( channel_ != NULL) {
        channel_->Close();
        delete channel_;
        channel_ = NULL;
    }
    state_ = STATE_IDLE;
    return 0;
}

void RtpSinker::doListen() {
    assert(channel_ != NULL);
    
    channel_->Listen(url_);
}

void RtpSinker::OnChannelOpened(DataChannel *ch, const bool& isOK) {
    // in streaming thread
    if ( isOK ) {
        state_ = STATE_SINKING;
    }
    
    SignalSinkingBegin(this, isOK);
}

void RtpSinker::OnChannelClosed(DataChannel *ch) {
    // in streaming thread
    state_ = STATE_IDLE;
    SignalSinkingEnd(this);
}

void RtpSinker::OnChannelDataRead(DataChannel *ch, const unsigned char* data, const unsigned int& length, bool& isValid) {
    // handling RTP package
	unsigned int ssrc;
	cricket::GetRtpSsrc(data, length, &ssrc);
	if ( ssrc != rtpSSRC_) {
        isValid = false;
        return;
	}
    isValid = true;

    unsigned char payloadType = data[kRTPHeaderSize] & 0x1F;
    unsigned char nalFlag = data[kRTPHeaderSize+1] & 0xE0;
    if ( payloadType == 28) {       //FA-U
        if ( nalFlag & 0x80) {      //first rtp package of a new FA-U NAL
            if ( buffer_->BufferSize() > 0) {
                buffer_->Reset();   //FIXME: we drop some RTP because new NAL is incoming
            }
            buffer_->PushBuffer(data, length);
        } else if ( nalFlag & 0x40) {
            // TODO , one coded picture is OK.
        } else {
            buffer_->PushBuffer(data, length);
        }
    } else {                        //Singla NAL
        // TODO , one coded picture is OK.
    }
    
}

