#include <string.h>
#include "mediabuffer.h"


MediaBuffer::MediaBuffer(const unsigned int pkg_number,
                         const unsigned int pkg_size) {
  MediaPackage *pkg;

  pkg_size_ = pkg_size;

  for(unsigned int i = 0; i < pkg_number; i++) {
    pkg = new MediaPackage(pkg_size_);
    pkg_pool_.push_back(pkg);   
  }
  pkg_released = new MediaPackage(pkg_size_);
  pkg_released->length = 0;
  pkg_seq_ = 0;

}

MediaBuffer::~MediaBuffer(){
  MediaPackage *pkg;
  for(unsigned int i = 0; i < pkg_pool_.size(); i++) {
    pkg = pkg_pool_[i];
    delete pkg;
  }
  pkg_pool_.clear();

  delete pkg_released;
}

void MediaBuffer::Reset() {
  MediaPackage *pkg;
  
  while(!buffer_.empty()) {
    pkg = buffer_.front();
    buffer_.pop_front();
    pkg_pool_.push_back(pkg);
  } 
  buffer_.clear();

  pkg_seq_ = 0;
  pkg_released->length = 0;
}

bool MediaBuffer::PushBuffer(const unsigned char *d, const unsigned int len) {
  MediaPackage *pkg = NULL;
  pkg_seq_ ++;

  // check memory space.
  if ( len > pkg_size_ ) {
    return false;
  }

  if ( pkg_pool_.size() == 0) {
    return false; 
  }

  // this is valid push to buffer  
  {
    talk_base::CritScope lock(&mutex_);
    pkg = pkg_pool_.back();
    pkg_pool_.pop_back();    
  }
  pkg->length = len;
  pkg->seq = pkg_seq_;

  memcpy(pkg->data_, d, len);
  pkg->data = pkg->data_;
  {
    talk_base::CritScope lock(&mutex_);
    buffer_.push_back(pkg);
  }
  return true;

}

bool MediaBuffer::PullBuffer() {
  std::list<MediaPackage *> *pBuffer;
  std::vector<MediaPackage *> *pPool;
  pBuffer = &buffer_;
  pPool = &pkg_pool_;

  MediaPackage *pkg; 
  {
    talk_base::CritScope lock(&mutex_);

    if (pBuffer->size() == 0)
        return false;
  
    pkg = pBuffer->front();
    pBuffer->pop_front();
  } 

  pkg_released->seq = pkg->seq;  
  pkg_released->length = pkg->length;
  memcpy(pkg_released->data_, pkg->data_, pkg_released->length);
  pkg_released->data = pkg_released->data_;

  { 
    talk_base::CritScope lock(&mutex_);
    pPool->push_back(pkg);
  }

  return true;
}

MediaPackage * MediaBuffer::Released() {
    return pkg_released;
}
