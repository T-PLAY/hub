#ifndef QWIDGET_STREAMVIEW_H
#define QWIDGET_STREAMVIEW_H

#include <QObject>
#include <QWidget>
#include <QThread>
#include <server.h>

class Thread_InputStream : public QThread
{
    Q_OBJECT
public:
    // constructor
    explicit Thread_InputStream(QObject * parent = 0, int iStreamer = 0);
    ~Thread_InputStream();

signals:
//    void addViewStreamSignal(Streamer streamer);
//    void addViewStreamSignal(int iStreamer);
//    void addViewStreamSignal();
    void newImage();

public:
    // overriding the QThread's run() method
    void run();

    Stream::InitPacket mInitPacket;
    unsigned char * mData;
private:
    ClientSocket mSock;
    int mAcquisitionSize;
//    Ui::MainWindow * mUi;
};


class QWidget_StreamView : public QWidget
{
    Q_OBJECT

public:
    explicit QWidget_StreamView(QWidget *parent = 0, int iStreamer = 0);
    ~QWidget_StreamView();


    int iStreamer() const;

public slots:
    void newImage();
    void setImage(unsigned char *img_ptr, int width, int height);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
//    QImage image;
//    uchar * imagePtr = nullptr;
//    static const int imgSize = 118;
    int m_iStreamer;

    int mWidth = 0;
    int mHeight = 0;
    unsigned char * img = nullptr;

    Thread_InputStream mThread;

};


#endif // QWIDGET_STREAMVIEW_H
