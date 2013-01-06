#ifndef _MEDIACHANNEL_H_
#define _MEDIACHANNEL_H_

#include "talk/base/sigslot.h"
#include "talk/base/thread.h"

class MediaChannel  : public sigslot::has_slots<> {
public:
  MediaChannel(){
    is_ready_ = false;
  }
  virtual ~MediaChannel(){};

  virtual bool Connect(const std::string &url) = 0;
  virtual int PushData(const unsigned char * data, size_t len) = 0;
  virtual bool Close() = 0;
  virtual bool IsReady() {
    return is_ready_;
  }
  
  sigslot::signal1<MediaChannel*, const bool&> SignalChannelOpened;
  sigslot::signal1<MediaChannel*> SignalChannelClosed;
  sigslot::signal3<MediaChannel*, const unsigned char*, const unsigned int&> SignalDataRead;
protected:
  bool is_ready_; 
};

MediaChannel * CreateChannel(const std::string &type, void *priv);

#endif
