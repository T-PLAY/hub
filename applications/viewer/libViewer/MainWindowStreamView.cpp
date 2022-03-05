#include "MainWindowStreamView.h"
#include "ui_MainWindowStreamView.h"

#include <cmath>

Thread_InputStream::Thread_InputStream(QObject* parent, std::string sensorName)
    : QThread(parent)
    , mInputStream(sensorName)
{
    std::cout << "Thread_InputStream()" << std::endl;

    size_t acquisitionSize = mInputStream.getAcquisitionSize();
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

//    bool serverRequestClose = false;
    try {

//        Stream::Acquisition acq = mInputStream.acquisition();
        Stream::Acquisition acq;
//        size_t acquisitionSize = mInputStream.getAcquisitionSize();

        while (!this->isInterruptionRequested()) {

            acq.mData = mData[m_iWriteBuffer];
            mInputStream >> acq;
            m_iReadBuffer = m_iWriteBuffer;
            m_iWriteBuffer = (m_iWriteBuffer + 1) % 2;

//            int dec = mData[m_iReadBuffer][0];
//            bool badImage = false;
//            for (size_t i = 0; i < acquisitionSize; ++i) {
//                if (mData[m_iReadBuffer][i] != (i / 192 + dec) % 256) {
//                    assert(false);
//                    badImage = true;
//                    break;
//                }
//            }
//            if (badImage) {
//                std::cout << "[streamView] error bad image" << std::endl;
//            }
            emit newImage();
        }

    } catch (std::exception& e) {
        std::cout << "[streamView] catch exception : " << e.what() << std::endl;
        return;
    }

    std::cout << "Thread_InputStream::run() end" << std::endl;
}

//////////////////////////////////////////////////////////////////////

MainWindowStreamView::MainWindowStreamView(QWidget* parent, std::string sensorName)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowStreamView)
    , mThread(this, sensorName)
    , mSensorName(sensorName)
{
    std::cout << "MainWindow::MainWindowStreamView(parent, " << sensorName << ")" << std::endl;

    ui->setupUi(this);


    assert(mThread.mInputStream.getDims().size() == 2);
    setMinimumWidth(mThread.mInputStream.getDims().at(0));
    setMinimumHeight(mThread.mInputStream.getDims().at(1));

    QObject::connect(&mThread, &Thread_InputStream::newImage, this, &MainWindowStreamView::newImage);

    mThread.start();
}

MainWindowStreamView::~MainWindowStreamView()
{
    std::cout << "~MainWindowStreamView()" << std::endl;

    delete ui;

    mThread.requestInterruption();
    std::cout << "~MainWindowStreamView() requested interruption" << std::endl;
    mThread.wait();
    std::cout << "~MainWindowStreamView() mThread join" << std::endl;

    emit onCloseStreamViewSignal(mSensorName);

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

    assert(mThread.mData[mThread.m_iReadBuffer] != nullptr);
    ui->centralwidget->setImage((unsigned char*)mThread.mData[mThread.m_iReadBuffer], mThread.mInputStream.getDims().at(0), mThread.mInputStream.getDims().at(1), mThread.mInputStream.getFormat());
}
