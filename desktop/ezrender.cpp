#include <QIODevice>
#include <QAudioOutput>
#include <QVideoSurfaceFormat>
#include "ffdefine.h"
#include "videowidget.h"
#include "ezrender.h"

EzRender::EzRender( VideoWidget* widget) {
    pWidget = widget;

	QAudioFormat format;
	// Set up the format, eg.
	format.setFrequency(8000);
	format.setChannels(1);
	format.setSampleSize(8);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::UnSignedInt);

	QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
	if (!info.isFormatSupported(format)) {
		printf("raw audio format not supported by backend, cannot play audio.\n");
		return;
	}

	audio = new QAudioOutput(format);
	audio->setBufferSize(128*1024);
	pAudioIO = audio->start();
}

EzRender::~EzRender() {    
	audio->stop();	
	delete audio;
}

#define SATURATE(a,min,max) ((a)<(min)?(min):((a)>(max)?(max):(a)))
static void YUV2RGB(unsigned char inY, unsigned char inU, unsigned char inV, unsigned char *R, unsigned char *G, unsigned char *B)
{
	int Gi = 0, Bi = 0;
	int Y = 9535*(inY-16);
	int U = inU - 128;
	int V = inV - 128;
	int Ri = (Y + 13074*V) >> 13;
	Ri = SATURATE(Ri, 0, 255);
	Gi = (Y - 6660*V - 3203*U) >> 13;
	Gi = SATURATE(Gi, 0, 255);
	Bi = (Y + 16531*U) >> 13;
	Bi = SATURATE(Bi, 0, 255);
	*R = Ri;
	*G = Gi;
	*B = Bi;
}


void EzRender::ShowPicture(VideoPicture *yuv) {
    int vWidth = yuv->width;
    int vHeight = yuv->height;

    QImage showedImage( vWidth, vHeight, QImage::Format_RGB32);
	for(int i = 0; i < vHeight; i++) {
		for(int j = 0; j < vWidth; j++) {
			unsigned char Y = *(yuv->vplan[0]+i*yuv->vplan_length[0]+j);
			unsigned char U = *(yuv->vplan[1]+i/2*yuv->vplan_length[1] + j/2);
			unsigned char V = *(yuv->vplan[2]+i/2*yuv->vplan_length[2] + j/2);
			unsigned char R,G,B;

			YUV2RGB(Y,U,V,&R,&G,&B);

			showedImage.setPixel(j,i,qRgb(R,G,B));
		}
	}
    //showedImage.save("./frame.png");

    QVideoFrame frame(showedImage);
    if (!frame.isValid())
        return;

    QAbstractVideoSurface *surface = pWidget->videoSurface();
    QVideoSurfaceFormat currentFormat = surface->surfaceFormat();
    
    if (frame.pixelFormat() != currentFormat.pixelFormat()
            || frame.size() != currentFormat.frameSize()) {
        QVideoSurfaceFormat format(frame.size(), frame.pixelFormat());
        if (!surface->start(format))
            return;
    }

    if (!surface->present(frame)) {
        surface->stop();
        return;
    } else {
        return;
    }

}


