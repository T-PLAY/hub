#include "MainWindowStreamView.h"
#include "ui_MainWindowStreamView.h"

#include <WidgetStreamView.h>
#include <cmath>
#include <socket.h>

Thread_InputStream::Thread_InputStream(QObject* parent, std::string sensorName)
    : QThread(parent)
//    , mInputStream(sensorName)
//    , mInputStream(ClientSocket(ClientSocket::Type::STREAM_VIEWER, sensorName))
//    , mInputStream(ClientSocket(sensorName, ""))
    , mInputStream(ClientSocket(sensorName, ""))
{
    std::cout << "Thread_InputStream()" << std::endl;
}

Thread_InputStream::~Thread_InputStream()
{
    std::cout << "~Thread_InputStream()" << std::endl;
}

void Thread_InputStream::run()
{
    std::cout << "Thread_InputStream::run()" << std::endl;

    try {

        while (!this->isInterruptionRequested()) {

            mInputStream >> mAcq;
            // std::cout << "receive acq : " << mAcq << std::endl;

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

    const auto dim = mThread.mInputStream.getDims().size();
    if (dim == 1) {
        ui->centralwidget = new WidgetStreamView1D(this);

        ui->centralwidget->setMinimumSize(350, 30);

    } else if (dim == 2) {

        ui->centralwidget = new WidgetStreamView2D(this);

        ui->centralwidget->setMinimumWidth(mThread.mInputStream.getDims().at(0));
        ui->centralwidget->setMinimumHeight(mThread.mInputStream.getDims().at(1));
    } else {
        std::cout << "unprocessed dimension" << std::endl;
        exit(1);
    }
    ui->centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
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

    assert(mThread.mAcq.mData != nullptr);
    (static_cast<WidgetStreamView*>(ui->centralwidget))->setData((unsigned char*)mThread.mAcq.mData, mThread.mInputStream.getDims(), mThread.mInputStream.getFormat());

    ++mCounterFps;
}
