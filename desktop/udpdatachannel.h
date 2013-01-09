#ifndef _UDPDATACHANNEL_H_
#define _UDPDATACHANNEL_H_

#include "datachannel.h"

namespace talk_base{
    class AsyncSocket;
};

class UdpDataChannel: public DataChannel { 
public:
    UdpDataChannel();
    virtual ~UdpDataChannel();
    
    virtual void Listen(const MediaURL& url);  
    virtual void Connect(const MediaURL& url);
    virtual int PushData(const unsigned char *data, size_t Len);
    virtual int Close();

protected:
    void OnConnectEvent(talk_base::AsyncSocket* socket);
    void OnCloseEvent(talk_base::AsyncSocket* socket, int err);
    void OnReadEvent(talk_base::AsyncSocket* socket);  
private:
    int createSocket();
    int bindSocket();

private:
    talk_base::AsyncSocket* udp_socket_;
    talk_base::SocketAddress remote_addr_;
    MediaURL my_url_; 
};

#endif
