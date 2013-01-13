#ifndef _EZRENDER_H_
#define _EZRENDER_H_

class VideoWidget;
class QIODevice;
class QAudioOutput;
class VideoPicture;

class EzRender {
public:
    EzRender( VideoWidget* pWidget);
    virtual ~EzRender();

    void ShowPicture(VideoPicture *yuv);

private:	
    VideoWidget *pWidget;	
    
    QIODevice *pAudioIO;
	QAudioOutput *audio;
};

#endif

