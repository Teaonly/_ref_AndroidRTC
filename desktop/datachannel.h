#ifndef _DATACHANNEL_H_
#define _DATACHANNEL_H_

#include "talk/base/sigslot.h"
#include "talk/base/thread.h"
#include "media.h"

class DataChannel  : public sigslot::has_slots<> {
public:
    enum State {
        STATE_DISCONNECTED = 0,
        STATE_CONNECTING = 1,
        STATE_LISTENING = 2,
        STATE_CONNECTED = 3,    
    };

    DataChannel(){
        state_ = STATE_DISCONNECTED;
    }
    virtual ~DataChannel(){};

    virtual void Listen(const MediaURL& url) = 0;
    virtual void Connect(const MediaURL& url) = 0;
    virtual int PushData(const unsigned char * data, size_t len) = 0;
    virtual int Close() = 0;

    virtual State GetState() {
        return state_;
    }

    sigslot::signal2<DataChannel*, const bool&> SignalChannelOpened;
    sigslot::signal1<DataChannel*> SignalChannelClosed;
    sigslot::signal4<DataChannel*, const unsigned char*, const unsigned int&, bool&> SignalDataRead;

protected:
    State state_;
};

DataChannel * CreateChannel(const std::string &type, void *priv);

#endif
