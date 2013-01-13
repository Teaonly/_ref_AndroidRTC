#ifndef _VIDEODIALOG_H_
#define _VIDEODIALOG_H_

#include <QtGui>

namespace Ui{
    class VideoDialogUI;
};

class VideoWidget;

class VideoDialog : public QDialog {
    Q_OBJECT

public:
    VideoDialog(QApplication *);
    ~VideoDialog();
    
    VideoWidget* GetVideoWidget();

protected:
    void closeEvent(QCloseEvent *event);   

private:
    void initGui();
    void setupSignal();

private:  
    Ui::VideoDialogUI *ui;    
};



#endif
