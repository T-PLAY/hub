#include "MainWindowStreamView.h"
#include "ui_MainWindowStreamView.h"

#include <WidgetStreamView.h>
#include <cmath>

Thread_InputStream::Thread_InputStream(QObject* parent, std::string sensorName)
    : QThread(parent)
    , mInputStream(sensorName)
{
    std::cout << "Thread_InputStream()" << std::endl;

    //    size_t acquisitionSize = mInputStream.getAcquisitionSize();
    //    mData[0] = new unsigned char[acquisitionSize];
    //    mData[1] = new unsigned char[acquisitionSize];
    //    mData = new unsigned char[acquisitionSize];
}

Thread_InputStream::~Thread_InputStream()
{
    std::cout << "~Thread_InputStream()" << std::endl;
    //    delete[] mData[0];
    //    delete[] mData[1];
    //    delete mData;
}

void Thread_InputStream::run()
{
    std::cout << "Thread_InputStream::run()" << std::endl;

    try {

        //        Stream::Acquisition acq;
        //        acq.mData = mData;

        while (!this->isInterruptionRequested()) {

            //            acq.mData = mData[m_iWriteBuffer];
            //            acq.mData = mData;
            mInputStream >> mAcq;
            // std::cout << "receive acq : " << mAcq << std::endl;

            //            auto dec = mAcq.mBackendTimeOfArrival - mAcq.mBackendTimestamp - 1'000'000 / 40;
            //            if (dec > 10'000) {
            //                std::cout << "not 40Hz dec = " << dec << std::endl;
            //            }

            //            m_iReadBuffer = m_iWriteBuffer;
            //            m_iWriteBuffer = (m_iWriteBuffer + 1) % 2;

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

    resize(1, 1);

    assert(ui->centralwidget != nullptr);
    delete ui->centralwidget;

    const int dim = mThread.mInputStream.getDims().size();
    if (dim == 1) {
        ui->centralwidget = new WidgetStreamView1D(this);

        ui->centralwidget->setMinimumSize(350, 30);
        //        ui->centralwidget->setMinimumWidth(100);
        //        ui->centralwidget->setMinimumHeight(30);

    } else if (dim == 2) {

        ui->centralwidget = new WidgetStreamView2D(this);

        ui->centralwidget->setMinimumWidth(mThread.mInputStream.getDims().at(0));
        ui->centralwidget->setMinimumHeight(mThread.mInputStream.getDims().at(1));
    } else {
        std::cout << "unprocessed dimension" << std::endl;
        exit(1);
    }
    ui->centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    //    auto policy = QSizePolicy::Maximum;
    //    QSizePolicy sizePolicy1(policy, policy);
    //    ui->centralwidget->setSizePolicy(sizePolicy1);
    setCentralWidget(ui->centralwidget);

    QObject::connect(&mThread, &Thread_InputStream::newImage, this, &MainWindowStreamView::newImage);

    mThread.start();
    mStartFps = std::chrono::high_resolution_clock::now();
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
    if (mCounterFps == std::ceil(mFps)) {
        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - mStartFps).count();
        mFps = (1'000'000.0 * std::ceil(mFps)) / duration;
        ui->statusbar->showMessage((std::string("fps:") + std::to_string(mFps)).c_str());
        mCounterFps = 0;
        mStartFps = std::chrono::high_resolution_clock::now();
    }

    //    assert(mThread.mData[mThread.m_iReadBuffer] != nullptr);
    assert(mThread.mAcq.mData != nullptr);
    //    ui->centralwidget->setImage((unsigned char*)mThread.mData[mThread.m_iReadBuffer], mThread.mInputStream.getDims().at(0), mThread.mInputStream.getDims().at(1), mThread.mInputStream.getFormat());
    (static_cast<WidgetStreamView*>(ui->centralwidget))->setData((unsigned char*)mThread.mAcq.mData, mThread.mInputStream.getDims(), mThread.mInputStream.getFormat());

    ++mCounterFps;
}
