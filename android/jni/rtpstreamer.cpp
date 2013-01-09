#include <assert.h>
#include "helper.h"
#include "rtpstreamer.h"
#include "mediachannel.h"
#include "h264encoder.h"
#include "mediabuffer.h"
#include "talk/session/phone/rtputils.h"

enum {
    MSG_START_CONNECT,
    MSG_CHECK_CONNECT,
    MSG_STREAMING_TIMER,
};

const int kMaxRTPSize = 1350;
const int kRTPHeaderSize = 12;
const int kMaxPayloadSize = kMaxRTPSize - kRTPHeaderSize;

RtpStreamer::RtpStreamer(talk_base::Thread* streaming_thread, talk_base::Thread* encoding_thread) {
    state_ = STATE_IDLE;
    channel_ = NULL;
    encoder_ = NULL;
    streaming_thread_ = streaming_thread;
    encoding_thread_ = encoding_thread;
    
    buffer_ = new MediaBuffer(64, kMaxRTPSize+128); 
}

RtpStreamer::~RtpStreamer() {
    StopStreaming();
    streaming_thread_->Quit();
    encoding_thread_->Quit();
    delete buffer_;
}

void RtpStreamer::OnMessage(talk_base::Message *msg) {
    if(msg->message_id == MSG_START_CONNECT ) {
        doConnect();
    } else if ( msg->message_id == MSG_STREAMING_TIMER) {
        doStreaming();
    }
}

int RtpStreamer::StartStreaming(const std::string& url, const std::string& description, const unsigned int& ssrc) {
    if ( url_.Parse(url) == false)
        return -1;

    if ( description_.Parse(description) == false)
        return -1;
    
    if ( description_.isAudio ) {
        // We don't support current
        return -1;
    }
   
    rtpSSRC_ = ssrc;
    rtpSeq_ = 0;

    //create encoder and channel object
    channel_ = CreateChannel(url_.proto, NULL);
    if ( channel_ == NULL) {
        return -1;
    }
    channel_->SignalChannelOpened.connect(this, &RtpStreamer::OnChannelOpened);
    channel_->SignalChannelClosed.connect(this, &RtpStreamer::OnChannelClosed);
    channel_->SignalDataRead.connect(this, &RtpStreamer::OnChannelDataRead);

    encoder_ = new H264Encoder(encoding_thread_);
    encoder_->SignalCodedNAL.connect(this, &RtpStreamer::OnCodedNAL);
    
    if ( encoder_->Prepare(description_) < 0) {
        delete encoder_;
        encoder_ = NULL;
        return -1;
    }

    buffer_->Reset();
    state_ = STATE_IDLE;
    streaming_thread_ ->Post(this, MSG_START_CONNECT);

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

void RtpStreamer::doStreaming() {
    if ( state_ == STATE_STREAMING ) {
        if ( buffer_->PullBuffer() ) {
            MediaPackage* pkg = buffer_->Released();
            LOGD("Sending rtp package len=%d", pkg->length);
            channel_->PushData(pkg->data, pkg->length);
        }
        streaming_thread_->PostDelayed(10, this, MSG_STREAMING_TIMER);
    } 
}

int RtpStreamer::ProvideCameraFrame(unsigned char *yuvData) {
    // in the calling thread
    if ( state_ != STATE_STREAMING) {
        return -1;
    }
    
    encoder_->EncodePicture(yuvData);
    return 0;
}


void RtpStreamer::OnCodedNAL(H264Encoder* enc, x264_nal_t* nal, const unsigned int& ts) {
    if ( description_.isVideo == false)
        return;

    // in encoding thread 
    unsigned char rtp_package[1024*2];
   
    int nalDataLen = nal->i_payload - (nal->b_long_startcode/8);
    if ( nalDataLen <= kMaxPayloadSize ) {
        // one rtp  one NAL
        cricket::SetRtpHeaderFlags(rtp_package, 0, 0, 0, 0); //padding = 0, externsion = 0, crsc_count = 0
        cricket::SetRtpPayloadType(rtp_package, 0, 35);             //H.264
        cricket::SetRtpSeqNum(rtp_package, 0, rtpSeq_); 
        cricket::SetRtpTimestamp(rtp_package, 0, ts);
        cricket::SetRtpSsrc(rtp_package, 0, rtpSSRC_);
        memcpy(&rtp_package[kRTPHeaderSize], nal->p_payload + (nal->b_long_startcode/8), nalDataLen);
        buffer_->PushBuffer(rtp_package, nalDataLen + kRTPHeaderSize);
        rtpSeq_++;
        return;
    } else {
        // n rtp 1 NAL
        int nPkgs = (nal->i_payload - (nal->b_long_startcode)/8) / kMaxPayloadSize;
        if (  (nal->i_payload - (nal->b_long_startcode)/8) % kMaxPayloadSize ) {
            nPkgs ++;
        }

        unsigned char* nalData = nal->p_payload + (nal->b_long_startcode/8) + 1;
        for(int i = 0; i < nPkgs; i++) {
            int currentPayloadLength = kMaxPayloadSize;
            if ( i == nPkgs -1 ) {
                currentPayloadLength = (nal->i_payload - (nal->b_long_startcode)/8) % kMaxPayloadSize;
            }

            cricket::SetRtpHeaderFlags(rtp_package, 0, 0, 0, 0); 
            cricket::SetRtpPayloadType(rtp_package, 0, 35); 
            cricket::SetRtpSeqNum(rtp_package, 0, rtpSeq_); 
            cricket::SetRtpTimestamp(rtp_package, 0, ts); 
            cricket::SetRtpSsrc(rtp_package, 0, rtpSSRC_); 

            // FU-A : FU indicator 
            rtp_package[kRTPHeaderSize] = nal->p_payload[ (nal->b_long_startcode/8) ];
            rtp_package[kRTPHeaderSize] = rtp_package[kRTPHeaderSize] & 0xE0;
            rtp_package[kRTPHeaderSize] = rtp_package[kRTPHeaderSize] + 28;
            // FU-A : FU header
            rtp_package[kRTPHeaderSize+1] = nal->p_payload[ (nal->b_long_startcode/8) ];
            rtp_package[kRTPHeaderSize+1] = rtp_package[kRTPHeaderSize+1] & 0x3F;
            if (i == 0) {
                rtp_package[kRTPHeaderSize+1] = rtp_package[kRTPHeaderSize+1] | 0x80;
            } else if ( i == (nPkgs - 1) ) {
                rtp_package[kRTPHeaderSize+1] = rtp_package[kRTPHeaderSize+1] | 0x40;
            }
            memcpy(&rtp_package[kRTPHeaderSize+2], nalData + i*kMaxPayloadSize, currentPayloadLength );
            buffer_->PushBuffer(rtp_package, currentPayloadLength + kRTPHeaderSize + 2);

            rtpSeq_++;
        }
    }
}

void RtpStreamer::OnChannelOpened(MediaChannel *ch, const bool& isOK) {
    // in streaming thread
    if ( isOK ) {
        state_ = STATE_STREAMING;
        streaming_thread_->PostDelayed(10, this, MSG_STREAMING_TIMER);
    }
    
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

