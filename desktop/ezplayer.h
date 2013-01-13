#ifndef _EZPLAYER_H_
#define _EZPLAYER_H_

#include "talk/base/sigslot.h"
#include "talk/base/thread.h"
#include "talk/base/messagequeue.h"
#include "talk/base/criticalsection.h"

class MediaBuffer;
class EzRender;
class FFDecoder;
class RtpSinker;

class EzPlayer : public sigslot::has_slots<>, public talk_base::MessageHandler {
public:
    EzPlayer(EzRender* render, talk_base::Thread* decoding_thread);
    virtual ~EzPlayer();
    
    void OnCodedPicture(RtpSinker* sink, const unsigned char *codedPicture, const unsigned int& length, const unsigned int& timeStamp);

protected:
    virtual void OnMessage(talk_base::Message *msg);

private:	
    EzRender* render_;	
    talk_base::Thread* decoding_thread_; 
    
    MediaBuffer* buffer_;    
    FFDecoder* decoder_;
};

#endif

