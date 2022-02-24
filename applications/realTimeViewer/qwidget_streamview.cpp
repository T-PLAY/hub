#include "qwidget_streamview.h"

#include <QPainter>

#include <iostream>
#include <server.h>

Thread_InputStream::Thread_InputStream(QObject* parent, int iStreamer)
    : QThread(parent)
    , mSock("127.0.0.1")
{
    std::cout << "Thread_InputStream()" << std::endl;

    mSock.write(Client::Type::STREAM_VIEWER);

    mSock.write(iStreamer);

    //    Streamer streamer;
    mSock.read(mInitPacket);
    std::cout << "device:" << (int)mInitPacket.mDevice << std::endl;
    std::cout << "format:" << (int)mInitPacket.mFormat << std::endl;
    std::cout << "height:" << (int)mInitPacket.mHeight << std::endl;
    std::cout << "sensor:" << (int)mInitPacket.mSensor << std::endl;
    std::cout << "width:" << (int)mInitPacket.mWidth << std::endl;

    mAcquisitionSize = mInitPacket.mWidth
        * mInitPacket.mHeight
        * Stream::formatNbByte[static_cast<int>(mInitPacket.mFormat)];
    mData = new unsigned char[mAcquisitionSize];
}

Thread_InputStream::~Thread_InputStream()
{
    std::cout << "~Thread_InputStream()" << std::endl;
    delete[] mData;
}

void Thread_InputStream::run()
{
    std::cout << "Thread_InputStream::run()" << std::endl;
    //        Hub hub("127.0.0.1", 4043);
    //    ClientSocket sock("127.0.0.1", 4043);

    bool serverRequestClose = false;
    try {
        while (!this->isInterruptionRequested() && !serverRequestClose) {
            Server::Message serverMessage;
            mSock.read(serverMessage);

            switch (serverMessage) {
            case Server::Message::DATA: {
                Stream::TimestampInterval timestampInterval;
                std::cout << "[streamView] read data" << std::endl;
                mSock.read(timestampInterval);

                mSock.read(mData, mAcquisitionSize);

                constexpr int width = 192;
                constexpr int height = 512;
                assert(mAcquisitionSize == 192 * 512);
                int dec = mData[0];
                for (int i = 0; i < width; ++i) {
                    for (int j = 0; j < height; ++j) {
                        const int tmp = mData[i + j * width];
//                        assert(tmp == j + dec);
                    }
                }
                emit newImage();
                //        //        Server::Message serverMessage;
                //        //        sock.read(serverMessage);
                //                    std::cout << "[streamer] server ping" << std::endl;
                break;
            }
            case Server::Message::CLOSE:
                std::cout << "[streamView] server request close" << std::endl;
                serverRequestClose = true;
                break;

            default:
                std::cout << "unknown message from server" << std::endl;
                exit(1);
            }
        }
    } catch (std::exception& e) {
        std::cout << "[streamView] catch exception : " << e.what() << std::endl;
    }

    //    if (this->isInterruptionRequested() && !serverRequestClose) {
    mSock.write(Client::Message::CLOSE);
    Server::Message serverMessage;
    mSock.read(serverMessage);
    assert(serverMessage == Server::Message::OK);
    //    }
}

QWidget_StreamView::QWidget_StreamView(QWidget* parent, int iStreamer)
    : QWidget(parent)
    , m_iStreamer(iStreamer)
    , mThread(this, iStreamer)
{
    setAttribute(Qt::WA_NoSystemBackground, true);

    setMinimumWidth(mThread.mInitPacket.mWidth);
    setMinimumHeight(mThread.mInitPacket.mHeight);

    QObject::connect(&mThread, &Thread_InputStream::newImage, this, &QWidget_StreamView::newImage);

    mThread.start();
}

QWidget_StreamView::~QWidget_StreamView()
{
    std::cout << "~QWidget_StreamView()" << std::endl;
    //    mThread.terminate();
    //    mThread.quit();
    //    mThread.wait();
    mThread.requestInterruption();
    std::cout << "~QWidget_StreamView() requested interruption" << std::endl;
    mThread.wait();
    std::cout << "~QWidget_StreamView() mThread join" << std::endl;
}

void QWidget_StreamView::newImage()
{
    setImage((unsigned char*)mThread.mData, mThread.mInitPacket.mWidth, mThread.mInitPacket.mHeight);
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

int QWidget_StreamView::iStreamer() const
{
    return m_iStreamer;
}
