#ifndef _H264ENCODER_H_
#define _H264ENCODER_H_

#include "talk/base/sigslot.h"
#include "talk/base/thread.h"
#include "talk/base/messagequeue.h"
#include "talk/base/criticalsection.h"
#include "talk/base/buffer.h"
#include "helper.h"

#include <stdint.h>
#include <inttypes.h>
extern "C" {
#include <x264.h>
}

class H264Encoder : public sigslot::has_slots<>, public talk_base::MessageHandler {
public:    
    H264Encoder(talk_base::Thread* thread);
    ~H264Encoder();
    
    int Prepare(const MediaDescription& desc);
    int EncodePicture(unsigned char *yuv);
    int Release();
    
    sigslot::signal3<H264Encoder* , x264_nal_t*, const unsigned int&> SignalCodedNAL;

protected:
    virtual void OnMessage(talk_base::Message *msg);
    void doEncoding();     

private:
    talk_base::CriticalSection mutex_;
    unsigned int pciture_enc_;
    int ppIndex;
    
    talk_base::Thread *encoding_thread_;
    x264_param_t x264_opt_;;
    x264_t *x264_hdl_;
    
    x264_picture_t x264_picin_[2];

    x264_picture_t x264_picout_;
};

#endif
