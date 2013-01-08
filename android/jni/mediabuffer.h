#ifndef _MEDIABUFFER_H_
#define _MEDIABUFFER_H_

#include <list>
#include <vector>
#include "talk/base/sigslot.h"
#include "talk/base/criticalsection.h"

const int MUX_OFFSET = 32;

struct MediaPackage{
public:
  MediaPackage(const unsigned int size) {
	data_ = new unsigned char[size + MUX_OFFSET];
    data_ += MUX_OFFSET;
  };
  ~MediaPackage() {
    if ( data != NULL) {
      data_ = data_ - MUX_OFFSET;
      delete data_;
    }
  }
  
  unsigned char *data_;
  unsigned char *data;
  unsigned int length;
  unsigned int seq;
};

class MediaBuffer: public sigslot::has_slots<> {
public:
  MediaBuffer(const unsigned int num, const unsigned int size);
  ~MediaBuffer();
  void Reset();

  // access from diffrent threads, they are safe.
  bool PushBuffer(const unsigned char *d, const unsigned int len);
  bool PullBuffer();
  MediaPackage *Released();

  unsigned int BufferSize(){
    return buffer_.size();
  }

private:
  unsigned int pkg_size_;
  unsigned int pkg_seq_;
  
  std::list<MediaPackage*> buffer_;
  std::vector<MediaPackage*> pkg_pool_;
  MediaPackage *pkg_released;
  
  talk_base::CriticalSection mutex_; 
};

#endif
