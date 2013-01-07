#ifndef _UDPMEDIACHANNEL_H_
#define _UDPMEDIACHANNEL_H_

#include "mediachannel.h"

namespace talk_base{
    class AsyncSocket;
};

class UdpMediaChannel: public MediaChannel { 
public:
    UdpMediaChannel();
    virtual ~UdpMediaChannel();
  
    virtual void Connect(const MediaURL& url);
    virtual int PushData(const unsigned char *data, size_t Len);
    virtual int Close();

protected:
    void OnConnectEvent(talk_base::AsyncSocket* socket);
    void OnCloseEvent(talk_base::AsyncSocket* socket, int err);
    void OnReadEvent(talk_base::AsyncSocket* socket);  
private:
    int createSocket();

private:
    talk_base::AsyncSocket* udp_socket_;
    talk_base::SocketAddress remote_addr_;
    MediaURL my_url_; 
};

#endif
