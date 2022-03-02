#include "MainWindowStreamView.h"
#include "ui_MainWindowStreamView.h"

#include <cmath>

Thread_InputStream::Thread_InputStream(QObject* parent, std::string sensorName)
    : QThread(parent)
    //    , mSock("127.0.0.1")
    , mInputStream(sensorName)
{
    std::cout << "Thread_InputStream()" << std::endl;

    //    mSock.write(Client::Type::STREAM_VIEWER);

    //    mSock.write(iStreamer);

    //    //    Streamer streamer;
    //    mSock.read(mInitPacket);
    //    std::cout << "device:" << (int)mInitPacket.mDevice << std::endl;
    //    std::cout << "format:" << (int)mInitPacket.mFormat << std::endl;
    //    std::cout << "height:" << (int)mInitPacket.mHeight << std::endl;
    //    std::cout << "sensor:" << (int)mInitPacket.mSensor << std::endl;
    //    std::cout << "width:" << (int)mInitPacket.mWidth << std::endl;

    size_t acquisitionSize = mInputStream.getAcquisitionSize();
    //    mAcquisitionSize = mInitPacket.mWidth
    //        * mInitPacket.mHeight
    //        * Stream::formatNbByte[static_cast<int>(mInitPacket.mFormat)];
    mData[0] = new unsigned char[acquisitionSize];
    mData[1] = new unsigned char[acquisitionSize];
}

Thread_InputStream::~Thread_InputStream()
{
    std::cout << "~Thread_InputStream()" << std::endl;
    delete[] mData[0];
    delete[] mData[1];
}

void Thread_InputStream::run()
{
    std::cout << "Thread_InputStream::run()" << std::endl;
    //        Hub hub("127.0.0.1", 4043);
    //    ClientSocket sock("127.0.0.1", 4043);

    bool serverRequestClose = false;
    try {
        while (!this->isInterruptionRequested() && !serverRequestClose) {
            Stream::Acquisition acq;
            acq.data = mData[m_iWriteBuffer];

            mInputStream >> acq;
            m_iReadBuffer = m_iWriteBuffer;
            m_iWriteBuffer = (m_iWriteBuffer + 1) % 2;

            int dec = mData[m_iReadBuffer][0];
            bool badImage = false;
            for (size_t i = 0; i < mInputStream.getAcquisitionSize(); ++i) {
                //                for (int i = 0; i < width; ++i) {
                //                    for (int j = 0; j < height; ++j) {
                //                        const int tmp = mData[m_iReadBuffer][i];
                //                        if (tmp != (j + dec) % 256) {
                if (mData[m_iReadBuffer][i] != (i / 192 + dec) % 256) {
                    //                        if (tmp != dec) {
                    //                            std::cout << "[streamView] error bad image" << std::endl;
                    badImage = true;
                    break;
                }
                //                        assert(tmp == j + dec);
                //                }
                //                if (badImage)
                //                    break;
            }
            if (badImage) {
                std::cout << "[streamView] error bad image" << std::endl;
            }
            emit newImage();
        }

    } catch (std::exception& e) {
        std::cout << "[streamView] catch exception : " << e.what() << std::endl;
        return;
    }
}

//////////////////////////////////////////////////////////////////////

MainWindowStreamView::MainWindowStreamView(QWidget* parent, std::string sensorName)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowStreamView)
    //    , m_iStreamer(sensorName)
    , mThread(this, sensorName)
//    , ui(new Ui::MainWindowStreamView)
{
    std::cout << "MainWindow::MainWindowStreamView(parent, " << sensorName << ")" << std::endl;

    ui->setupUi(this);

    //    setAttribute(Qt::WA_NoSystemBackground, true);

    assert(mThread.mInputStream.getDims().size() == 2);
    setMinimumWidth(mThread.mInputStream.getDims().at(0));
    setMinimumHeight(mThread.mInputStream.getDims().at(1));

    QObject::connect(&mThread, &Thread_InputStream::newImage, this, &MainWindowStreamView::newImage);

    mThread.start();

    //    ui->menubar->addAction("File");
    //    ui->statusbar->showMessage("hello");
    //    QStatusBar * statusBar = new QStatusBar(this);
    //    statusBar->showMessage("hello");
}

MainWindowStreamView::~MainWindowStreamView()
{
    delete ui;

    std::cout << "~MainWindowStreamView()" << std::endl;
    //    mThread.terminate();
    //    mThread.quit();
    //    mThread.wait();
    mThread.requestInterruption();
    std::cout << "~MainWindowStreamView() requested interruption" << std::endl;
    mThread.wait();
    std::cout << "~MainWindowStreamView() mThread join" << std::endl;
}

std::string MainWindowStreamView::getStreamerSensorName() const
{
    return mThread.mInputStream.getSensorName();
}

void MainWindowStreamView::newImage()
{
    if (mCounterFps == 0) {
        mStartFps = std::chrono::high_resolution_clock::now();
    } else if (mCounterFps == std::ceil(mFps)) {
        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - mStartFps).count();
        mFps = (1000.0 * std::ceil(mFps)) / duration;
        ui->statusbar->showMessage((std::string("fps:") + std::to_string(mFps)).c_str());
        mCounterFps = -1;
    }
    ++mCounterFps;

    ui->centralwidget->setImage((unsigned char*)mThread.mData[mThread.m_iReadBuffer], mThread.mInputStream.getDims().at(0), mThread.mInputStream.getDims().at(1));

    //    mThread.m_iReadBuffer = (mThread.m_iReadBuffer + 1) % 2;
    //    mtx.unlock();
}
