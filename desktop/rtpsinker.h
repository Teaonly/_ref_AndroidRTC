#ifndef _RTPSINKER_H_
#define _RTPSINKER_H_

#include "media.h"
#include "talk/base/sigslot.h"
#include "talk/base/thread.h"
#include "talk/base/messagequeue.h"
#include "talk/base/criticalsection.h"
#include "talk/base/buffer.h"

class DataChannel;
class MediaBuffer;

class RtpSinker: public sigslot::has_slots<>, public talk_base::MessageHandler {
public:
    RtpSinker(talk_base::Thread* sreaming_thread);
    ~RtpSinker();
    
    int StartSinking(const std::string& url, const std::string& description, const unsigned int& ssrc);
    int StopSinking();
     
    sigslot::signal2<RtpSinker*, const bool&> SignalSinkingBegin;
    sigslot::signal1<RtpSinker*> SignalSinkingEnd;
    sigslot::signal4<RtpSinker*, const unsigned char*, const unsigned int&, const unsigned int&> SignalCodedPicture;

protected:
    virtual void OnMessage(talk_base::Message *msg);

    void OnChannelOpened(DataChannel *ch, const bool& isOK);
    void OnChannelClosed(DataChannel *ch);
    void OnChannelDataRead(DataChannel *ch, const unsigned char* data, const unsigned int& length, bool& isValid);
    void doListen();
    
    void convertSignalNAL();
    void convertMultipulNAL();

private: 
    enum SinkingState{
        STATE_IDLE,
        STATE_WAITTING,
        STATE_SINKING
    };

    SinkingState state_;
    DataChannel* channel_;
    talk_base::Thread *streaming_thread_;

    MediaBuffer *buffer_;
    MediaURL url_;
    MediaDescription description_;    
    unsigned int rtpSSRC_;
};


#endif

