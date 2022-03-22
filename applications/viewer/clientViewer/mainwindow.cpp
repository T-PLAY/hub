#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMdiSubWindow>
#include <QStandardItemModel>
//#include <server.h>
#include <formsensorview.h>
#include <stream.h>

Thread_Client::Thread_Client(QObject* parent)
    : QThread(parent)
{
    std::cout << "[Thread_Client] Thread_Client()" << std::endl;
}

void Thread_Client::run()
{
    std::cout << "[Thread_Client] Thread_Client::run()" << std::endl;

    while (!this->isInterruptionRequested()) {
        try {
            ClientSocket sock("127.0.0.1", SERVICE_PORT);
            sock.write(ClientSocket::Type::VIEWER);

            while (!this->isInterruptionRequested()) {

                Socket::Message serverMessage;
                sock.read(serverMessage);

                switch (serverMessage) {

                case Socket::Message::PING: {
                    // server check client connection
                    // nothing to do
                } break;

                case Socket::Message::NEW_STREAMER: {
                    std::cout << "[Thread_Client] [viewer] new streamer" << std::endl;
                    std::string streamerSensorName;
                    sock.read(streamerSensorName);
                    std::string format;
                    sock.read(format);
                    std::string dims;
                    sock.read(dims);
                    std::string size;
                    sock.read(size);

                    std::cout << "[Thread_Client] [viewer] new streamer " << streamerSensorName << ", format:" << format << ", dims:" << dims << ", acquisitionSize:" << size << std::endl;
                    std::cout << "[Thread_Client] [viewer] emit addSensorSignal '" << streamerSensorName << "'" << std::endl;
                    emit addSensorSignal(streamerSensorName, format, dims, size);

                } break;

                case Socket::Message::DEL_STREAMER: {
                    std::string streamerSensorName;
                    sock.read(streamerSensorName);
                    std::cout << "[Thread_Client] [viewer] del streamer '" << streamerSensorName << "'" << std::endl;
                    emit delSensorSignal(streamerSensorName);
                } break;

                default:
                    std::cout << "[Thread_Client] unknown message from server" << std::endl;
                }
            }

        } catch (std::exception& e) {
            std::cout << "[Thread_Client] [viewer] catch exception : " << e.what() << std::endl;
        }
    } // while (!this->isInterruptionRequested())
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mThreadClient(this)
{

    ui->setupUi(this);

    QObject::connect(&mThreadClient, &Thread_Client::addSensorSignal, this, &MainWindow::addSensor);
    QObject::connect(&mThreadClient, &Thread_Client::delSensorSignal, this, &MainWindow::delSensor);
    mThreadClient.start();
}

MainWindow::~MainWindow()
{
    std::cout << "[MainWindow] ~MainWindow()" << std::endl;

    delete ui;
    std::cout << "[MainWindow] ~MainWindow() deleted ui" << std::endl;

    mThreadClient.requestInterruption();
    mThreadClient.wait();

    std::cout << "[MainWindow] ~MainWindow() mThreadClient.terminated()" << std::endl;

    for (auto& pair : mStreamViews) {
        auto* streamView = pair.second;

        assert(streamView != nullptr);
        std::cout << "[MainWindow] ~MainWindow() delete " << streamView << std::endl;
        delete streamView;
        streamView = nullptr;
    }
    mStreamViews.clear();
}

void MainWindow::addSensor(std::string streamerSensorName, std::string format, std::string dims, std::string size)
{
    std::cout << "[MainWindow] MainWindow::addSensor '" << streamerSensorName << "'" << std::endl;

    FormSensorView* sensorView = new FormSensorView(ui->centralwidget, streamerSensorName, format, dims, size);
    ui->verticalLayoutSensor->insertWidget(static_cast<int>(mSensorViews.size()), sensorView);
    mSensorViews[streamerSensorName] = sensorView;
    QObject::connect(sensorView, &FormSensorView::addViewStreamSignal, this, &MainWindow::addStreamView);
    QObject::connect(sensorView, &FormSensorView::delViewStreamSignal, this, &MainWindow::delStreamView);
}

void MainWindow::delSensor(std::string streamerSensorName)
{
    std::cout << "[MainWindow] MainWindow::delSensor '" << streamerSensorName << "'" << std::endl;

    delStreamView(streamerSensorName);

    assert(mSensorViews.find(streamerSensorName) != mSensorViews.end()); // todo: fix one time error
    FormSensorView* sensorView = mSensorViews.at(streamerSensorName);
    delete sensorView;
    mSensorViews.erase(streamerSensorName);

    std::cout << "[MainWindow] MainWindow::delSensor end '" << streamerSensorName << "'" << std::endl;
}

void MainWindow::addStreamView(std::string streamerSensorName)
{
    std::cout << "[MainWindow] MainWindow::addStreamView slot '" << streamerSensorName << "'"
              << ", nb streamView = " << mStreamViews.size() << std::endl;

    MainWindowStreamView* streamView = new MainWindowStreamView(ui->mdiArea, streamerSensorName);
    assert(mStreamViews.find(streamerSensorName) == mStreamViews.end());
    mStreamViews[streamerSensorName] = streamView;
    std::cout << "[MainWindow] MainWindow::addStreamView add " << streamView << std::endl;

    QMdiSubWindow* subWindow = ui->mdiArea->addSubWindow(streamView);
    subWindow->setVisible(true);

    subWindow->setWindowTitle(streamerSensorName.c_str());

    QObject::connect(streamView, &MainWindowStreamView::onCloseStreamViewSignal, this, &MainWindow::onCloseStreamView);
}

#include <typeinfo>

void MainWindow::delStreamView(std::string streamerSensorName)
{
    std::cout << "[MainWindow] MainWindow::delStreamView slot, nb streamView = " << mStreamViews.size() << std::endl;

    if (mStreamViews.find(streamerSensorName) != mStreamViews.end()) {
        MainWindowStreamView* streamView = mStreamViews.at(streamerSensorName);

        std::cout << "[MainWindow] MainWindow::delStreamView delete " << streamView << std::endl;

        ui->mdiArea->removeSubWindow(streamView->parentWidget());
        delete streamView;
        streamView = nullptr;
    }
    std::cout << "[MainWindow] MainWindow::delStreamView slot end, nb streamView = " << mStreamViews.size() << std::endl;
}

void MainWindow::onCloseStreamView(std::string streamerSensorName)
{
    std::cout << "[MainWindow] MainWindow::onCloseStreamView " << std::endl;
    assert(mSensorViews.find(streamerSensorName) != mSensorViews.end());
    mSensorViews.at(streamerSensorName)->setRadioButtonOff();

    assert(mStreamViews.find(streamerSensorName) != mStreamViews.end());
    mStreamViews.erase(streamerSensorName);
}
