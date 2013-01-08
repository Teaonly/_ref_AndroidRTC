#ifndef _RTPSTREAMER_H_
#define _RTPSTREAMER_H_

#include "helper.h"
#include "talk/base/sigslot.h"
#include "talk/base/thread.h"
#include "talk/base/messagequeue.h"
#include "talk/base/criticalsection.h"
#include "talk/base/buffer.h"

#include <stdint.h>
#include <inttypes.h>
extern "C" {
#include <x264.h>
}


class MediaChannel;
class H264Encoder;
class MediaBuffer;

class RtpStreamer: public sigslot::has_slots<>, public talk_base::MessageHandler {
public:
    RtpStreamer(talk_base::Thread* sreaming_thread, talk_base::Thread* encoding_thread);
    ~RtpStreamer();
    
    int StartStreaming(const std::string& url, const std::string& description, const unsigned int& ssrc);
    int ProvideCameraFrame(unsigned char *yuvData);
    int StopStreaming();
     
    sigslot::signal2<RtpStreamer*, const bool&> SignalStreamingBegin;
    sigslot::signal1<RtpStreamer*> SignalStreamingEnd;

protected:
    virtual void OnMessage(talk_base::Message *msg);
    void OnCodedNAL(H264Encoder* enc, x264_nal_t* nal, const unsigned int& ts);

    void OnChannelOpened(MediaChannel *ch, const bool& isOK);
    void OnChannelClosed(MediaChannel *ch);
    void OnChannelDataRead(MediaChannel *ch, const unsigned char* data, const unsigned int& length);

    void doConnect();

private: 
    enum StreamingState{
        STATE_IDLE,
        STATE_CONNECTING,
        STATE_STREAMING
    };

    StreamingState state_;
    MediaChannel* channel_;
    H264Encoder* encoder_;
    talk_base::Thread *streaming_thread_;
    talk_base::Thread *encoding_thread_;

    MediaBuffer *buffer_;
    MediaURL url_;
    MediaDescription description_;    
    unsigned int rtpSSRC_;
    unsigned short rtpSeq_;
};


#endif

