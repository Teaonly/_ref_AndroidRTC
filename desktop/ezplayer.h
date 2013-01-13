#ifndef _EZPLAYER_H_
#define _EZPLAYER_H_

#include "talk/base/sigslot.h"
#include "talk/base/thread.h"
#include "talk/base/messagequeue.h"
#include "talk/base/criticalsection.h"


class EzRender;
class FFDecoder;

class EzPlayer : public sigslot::has_slots<>, public talk_base::MessageHandler {
public:
    EzPlayer(EzRender* render, talk_base::Thread* decoding_thread);
    virtual ~EzPlayer();

private:	
    EzRender* render_;	
    talk_base::Thread* decoding_thread_; 
    
    FFDecoder* decoder_;
};

#endif

