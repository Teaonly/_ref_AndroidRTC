#include "tcpmediachannel.h"

MediaChannel * CreateChannel(const std::string &type, void *priv) {
    if ( type == "TCP" ) {
        return new TcpMediaChannel();
    }
    
    return NULL;
}

