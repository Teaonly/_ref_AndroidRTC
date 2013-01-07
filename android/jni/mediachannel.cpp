#include "tcpmediachannel.h"
#include "udpmediachannel.h"

MediaChannel * CreateChannel(const std::string &type, void *priv) {
    if ( type == "tcp" ) {
        return new TcpMediaChannel();
    } else if ( type == "udp" ) {
        return new UdpMediaChannel();
    }
    
    return NULL;
}

