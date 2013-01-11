#include <iostream>
#include "videodialog.h"
#include "ui_player.h"
#include "videowidget.h"

VideoDialog::VideoDialog(QApplication *application){
	initGui();
    
    setupSignal();	
}

VideoDialog::~VideoDialog(){  	
    // Free GUI resouce
    delete ui;
}

void VideoDialog::initGui(){ 
    ui = new Ui::VideoDialogUI();
	ui->setupUi(this); 
}

void VideoDialog::setupSignal(){
}

void VideoDialog::closeEvent(QCloseEvent *event) {    
    event->accept();
}

