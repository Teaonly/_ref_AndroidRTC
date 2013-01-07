#include <assert.h>
#include <iostream>
#include "talk/base/socketstream.h"
#include "talk/base/asyncsocket.h"
#include "udpmediachannel.h"
#include "helper.h"

UdpMediaChannel::UdpMediaChannel() {
    udp_socket_ = NULL;
}

UdpMediaChannel::~UdpMediaChannel() {
    Close();
}

int UdpMediaChannel::Close(){
    if( udp_socket_) {
        udp_socket_->SignalReadEvent.disconnect(this); 
        udp_socket_->Close();
        delete udp_socket_; 
        udp_socket_ = NULL;
    }
    return 0;
}

int UdpMediaChannel::createSocket() {
    talk_base::Thread* pth = talk_base::Thread::Current();
    talk_base::AsyncSocket* socket =
        pth->socketserver()->CreateAsyncSocket(SOCK_DGRAM);
    udp_socket_ = socket; 
    assert(udp_socket_);
    udp_socket_->SignalReadEvent.connect(this, &UdpMediaChannel::OnReadEvent);

    int port;
    std::string remote_addr;
    remote_addr = my_url_.location;
    port = atoi( my_url_.resource.c_str());

    talk_base::SocketAddress addr(remote_addr, port);
    remote_addr_ = addr;

    return 0;
}

void UdpMediaChannel::Connect(const MediaURL& url) {
    my_url_ = url;
    is_ready_ = false;
    Close();

    int ret = createSocket();

    if ( ret < 0) {
        Close();    
        SignalChannelOpened(this, false); 
        return;
    }

    is_ready_ = true;
    SignalChannelOpened(this, true); 
}

int UdpMediaChannel::PushData(const unsigned char *data, size_t len) {
    if ( udp_socket_ == NULL)
        return -1;

    if ( udp_socket_->GetState() == talk_base::Socket::CS_CONNECTING)
        return 0;

    int result = udp_socket_->SendTo(data, len, remote_addr_);
    if (result < 0) {
        if (udp_socket_->IsBlocking())
            return 0;
        return -1;
    }

    return result;
}

void UdpMediaChannel::OnReadEvent(talk_base::AsyncSocket* socket) {
    assert(socket == udp_socket_);

    unsigned char temp[2048];
    int ret = udp_socket_->Recv(temp, sizeof(temp) - 1);
    if (ret > 0) {
        SignalDataRead(this, temp, ret);
    }   
}

