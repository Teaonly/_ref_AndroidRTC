#include "udpdatachannel.h"

DataChannel * CreateChannel(const std::string &type, void *priv) {
    if ( type == "udp" ) {
        return new UdpDataChannel();
    }
    
    return NULL;
}

