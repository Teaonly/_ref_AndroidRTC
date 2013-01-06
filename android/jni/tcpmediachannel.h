#ifndef _TCPMEDIACHANNEL_H_
#define _TCPMEDIACHANNEL_H_

#include "mediachannel.h"

namespace talk_base{
    class AsyncSocket;
};

class TcpMediaChannel: public MediaChannel { 
public:
    TcpMediaChannel();
    virtual ~TcpMediaChannel();
  
    virtual void Connect(const MediaURL& url);
    virtual int PushData(const unsigned char *data, size_t Len);
    virtual int Close();

protected:
    void OnConnectEvent(talk_base::AsyncSocket* socket);
    void OnCloseEvent(talk_base::AsyncSocket* socket, int err);
    void OnReadEvent(talk_base::AsyncSocket* socket);  
private:
    int doConnect();
    void createSocket();

private:
    talk_base::AsyncSocket* tcp_socket_;
    MediaURL my_url_; 
};

#endif
