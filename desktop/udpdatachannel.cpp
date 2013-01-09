#include <assert.h>
#include <iostream>
#include "talk/base/socketaddress.h"
#include "talk/base/asyncsocket.h"
#include "udpdatachannel.h"
#include "media.h"

UdpDataChannel::UdpDataChannel() {
    udp_socket_ = NULL;
    remote_addr_.Clear();   //nil address 
}

UdpDataChannel::~UdpDataChannel() {
    Close();
}

int UdpDataChannel::Close(){
    if( udp_socket_) {
        udp_socket_->SignalReadEvent.disconnect(this); 
        udp_socket_->Close();
        delete udp_socket_; 
        udp_socket_ = NULL;
    }
    return 0;
}

int UdpDataChannel::createSocket() {
    talk_base::Thread* pth = talk_base::Thread::Current();
    talk_base::AsyncSocket* socket =
        pth->socketserver()->CreateAsyncSocket(SOCK_DGRAM);
    udp_socket_ = socket; 
    assert(udp_socket_);
    udp_socket_->SignalReadEvent.connect(this, &UdpDataChannel::OnReadEvent);

    return 0;
}

int UdpDataChannel::bindSocket() {
    int port = atoi( my_url_.resource.c_str());
    talk_base::SocketAddress addr;
    addr.SetPort(port);
    return udp_socket_->Bind(addr);  
}    

void UdpDataChannel::Listen(const MediaURL& url) {
   my_url_ = url;
    Close();

    int ret = createSocket();
    if ( ret < 0) {
        Close();    
        SignalChannelOpened(this, false); 
        return;
    }
    
    ret = bindSocket();
    if ( ret < 0) {
        Close();    
        SignalChannelOpened(this, false); 
        return;
    }
 
    state_ = STATE_CONNECTED; 
    SignalChannelOpened(this, true); 
}

void UdpDataChannel::Connect(const MediaURL& url) {
    // in streaming_thread 
    my_url_ = url;
    Close();

    int ret = createSocket();
    if ( ret < 0) {
        Close();    
        SignalChannelOpened(this, false); 
        return;
    }
    
    int port = atoi( my_url_.resource.c_str());
    std::string remote_addr;
    remote_addr = my_url_.location;
    talk_base::SocketAddress addr(remote_addr, port);
    remote_addr_ = addr;

    state_ = STATE_CONNECTED; 
    SignalChannelOpened(this, true); 
}

int UdpDataChannel::PushData(const unsigned char *data, size_t len) {
    if ( udp_socket_ == NULL)
        return -1;
    
    if ( udp_socket_->GetState() == talk_base::Socket::CS_CONNECTING)
        return 0;
    
    if ( remote_addr_.IsNil() )
        return 0;
     
    int result = udp_socket_->SendTo(data, len, remote_addr_);
    if (result < 0) {
        if (udp_socket_->IsBlocking())
            return 0;
        return -1;
    }
    return result;
}

void UdpDataChannel::OnReadEvent(talk_base::AsyncSocket* socket) {
    assert(socket == udp_socket_);

    unsigned char temp[2048];
    talk_base::SocketAddress addr;
    
    int ret = udp_socket_->RecvFrom(temp, sizeof(temp) - 1, &addr);
    if (ret > 0) {
        bool isValid = false;
        SignalDataRead(this, temp, ret, isValid);
        if ( isValid && !remote_addr_.IsNil() ) {
            remote_addr_ = addr;
        }
    }   
}

