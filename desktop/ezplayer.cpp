#include "ffdefine.h"
#include "mediabuffer.h"
#include "rtpsinker.h"
#include "ezplayer.h"
#include "ezrender.h"
#include "ffdecoder.h"

enum {
    MSG_NEW_PACKAGE,
};

const int kMaxPackageSize = 256*1024;

EzPlayer::EzPlayer(EzRender* render, talk_base::Thread* decoding_thread)  {
    render_ = render;
    decoding_thread_ = decoding_thread;

    buffer_ = new MediaBuffer(32, kMaxPackageSize); 
    picture_ = new VideoPicture();
    decoder_ = CreateH264Decoder();
}   


EzPlayer::~EzPlayer() {    
    if ( decoder_ != NULL) {
        delete decoder_;
    }
}

void EzPlayer::OnCodedPicture(RtpSinker* sink, const unsigned char *codedPicture, const unsigned int& length, const unsigned int& timeStamp) {
    buffer_->PushBuffer(codedPicture, length);
    decoding_thread_->Post(this, MSG_NEW_PACKAGE);
}

void EzPlayer::OnMessage(talk_base::Message *msg) {
    if(msg->message_id == MSG_NEW_PACKAGE ) {
        if ( buffer_->PullBuffer() ) {
            MediaPackage* pkg = buffer_->Released();
            if ( decoder_->DecodeVideoPacket(pkg, picture_) ) {
                render_->ShowPicture(picture_);
            }
        }         
    }
}


