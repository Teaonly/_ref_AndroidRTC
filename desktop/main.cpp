#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "talk/base/thread.h"
#include "rtpsinker.h"

int main(int argc, char *argv[]) {
    talk_base::Thread *streaming_thread = new talk_base::Thread();
    streaming_thread->Start();
    
    RtpSinker *sinker = new RtpSinker(streaming_thread);
    
    std::string localAddr = "udp://127.0.0.1:1979";
    std::string description = "video:h.264:640:480";
    sinker->StartSinking(localAddr, description, 0x19791010);
    
    talk_base::Thread::Current()->Run();
    return 0;
}
