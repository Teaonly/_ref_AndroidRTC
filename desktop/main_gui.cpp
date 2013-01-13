#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "talk/base/thread.h"
#include "rtpsinker.h"
#include "videodialog.h"
#include "ezplayer.h"
#include "ezrender.h"

class SinkerReceiver: public sigslot::has_slots<> {
public:    
    SinkerReceiver() {
    }
    ~SinkerReceiver() {
    }

    void OnCodedPicture(RtpSinker* sinker, const unsigned char* pic, const unsigned int& length, const unsigned int& ts) {
        static FILE* fp = NULL;
        if ( fp == NULL )
            fp = fopen("/tmp/test.h264", "wb");

        fwrite(pic, length, 1, fp);
    }
};

int main(int argc, char *argv[]) {
    talk_base::Thread *streaming_thread = new talk_base::Thread();
    streaming_thread->Start();
 
    talk_base::Thread *decoding_thread = new talk_base::Thread();
    decoding_thread->Start();
    
    RtpSinker *sinker = new RtpSinker(streaming_thread);
    SinkerReceiver *receiver = new SinkerReceiver();
    sinker->SignalCodedPicture.connect(receiver, &SinkerReceiver::OnCodedPicture);

    std::string localAddr = "udp://127.0.0.1:1979";
    std::string description = "video:h.264:640:480";
    sinker->StartSinking(localAddr, description, 0x19791010);
    
    QApplication app(argc, argv);
    VideoDialog vDialog(&app);
    
    EzRender* render = new EzRender(vDialog.GetVideoWidget());
    EzPlayer* player = new EzPlayer(render, decoding_thread);
    sinker->SignalCodedPicture.connect(player, &EzPlayer::OnCodedPicture);

    vDialog.show();
    return app.exec();
   
    //delete objects
}
