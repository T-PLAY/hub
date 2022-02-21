#include "qwidget_streamview.h"

#include <QPainter>

#include <iostream>
#include <server.h>

Thread_InputStream::Thread_InputStream(QObject* parent, int iStreamer)
    : QThread(parent)
    , m_iStreamer(iStreamer)
    , mSock("127.0.0.1", 4043)
{
    std::cout << "Thread_InputStream()" << std::endl;

    mSock.write(Client::Type::STREAM_VIEWER);

    mSock.write(m_iStreamer);

//    Streamer streamer;
    mSock.read(mInitPacket);
    std::cout << "device:" << (int)mInitPacket.mDevice << std::endl;
    std::cout << "format:" << (int)mInitPacket.mFormat << std::endl;
    std::cout << "height:" << (int)mInitPacket.mHeight << std::endl;
    std::cout << "sensor:" << (int)mInitPacket.mSensor << std::endl;
    std::cout << "width:" << (int)mInitPacket.mWidth << std::endl;

}

void Thread_InputStream::run()
{
    std::cout << "Thread_InputStream::run()" << std::endl;
    //        Hub hub("127.0.0.1", 4043);
//    ClientSocket sock("127.0.0.1", 4043);

//    while (true) {
//        //        Server::Message serverMessage;
//        //        sock.read(serverMessage);

//    }
}

QWidget_StreamView::QWidget_StreamView(QWidget* parent, int iStreamer)
    : QWidget(parent)
    , mThread(this, iStreamer)
{
    setAttribute(Qt::WA_NoSystemBackground, true);

    setMinimumWidth(mThread.mInitPacket.mWidth);
    setMinimumHeight(mThread.mInitPacket.mHeight);

    mThread.start();
}

void QWidget_StreamView::setImage(unsigned char* img_ptr, int pWidth, int pHeight)
{
    //    std::cout << "setImage " << width() << " " << height() << std::endl;
    img = img_ptr;
    mWidth = pWidth;
    mHeight = pHeight;
    update();
}

void QWidget_StreamView::paintEvent(QPaintEvent* event)
{
    //    std::cout << "paintEvent " << width() << " " << height() << std::endl;
    Q_UNUSED(event);
    QPainter painter;
    painter.begin(this);
    if (img != nullptr) {
        QImage image = QImage((unsigned char*)img, mWidth, mHeight, mWidth, QImage::Format_Grayscale8).scaled(this->size());
        const QPoint p = QPoint(0, 0);
        painter.drawImage(p, image);
    } else {
        painter.fillRect(0, 0, width(), height(), Qt::red);
    }
    painter.end();
}
