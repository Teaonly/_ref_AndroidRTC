#include <QIODevice>
#include <QAudioOutput>
#include <QVideoSurfaceFormat>
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

void EzRender::ShowPicture() {

}


