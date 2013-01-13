#include "ezplayer.h"
#include "ezrender.h"
#include "ffdecoder.h"

EzPlayer::EzPlayer(EzRender* render, talk_base::Thread* decoding_thread)  {
    render_ = render;
    decoding_thread_ = decoding_thread;
    decoder_ = CreateH264Decoder();
}   


EzPlayer::~EzPlayer() {    
    if ( decoder_ != NULL) {
        delete decoder_;
    }
}



