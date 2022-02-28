#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <thread>

//#include <QTcpSocket>
//#include <iostream>
#include <QDebug>
#include <QMdiSubWindow>
#include <QStandardItemModel>
#include <hub.h>
//#include <FormStreamView.h>
#include <server.h>

Thread_Client::Thread_Client(QObject* parent)
    : QThread(parent)
{
    std::cout << "Thread_Client()" << std::endl;
    //    QObject::connect(this, &Thread_Client::addViewStreamSignal, mainWindow, &MainWindow::addViewStream);
}

void Thread_Client::run()
{
    std::cout << "Thread_Client::run()" << std::endl;
    //        Hub hub("127.0.0.1", 4043);

    while (!this->isInterruptionRequested()) {
        try {
            ClientSocket sock("127.0.0.1");
            sock.write(Client::Type::VIEWER);

            while (!this->isInterruptionRequested()) {
                //                sock.write(Client::Message::PING);
                //                std::cout << "Thread_Client::run() read message" << std::endl;
                Server::Message serverMessage;
                sock.read(serverMessage);

                switch (serverMessage) {
                case Server::Message::PING: {
                    // server check client connection
                    // nothing to do
                    //                    std::cout << "Thread_Client::run() server ping" << std::endl;
                } break;
                case Server::Message::NEW_STREAMER:
                    std::cout << "[viewer] new streamer" << std::endl;
                    //            Streamer streamer;
                    int iStreamer;
                    sock.read(iStreamer);
                    //            sock.read(streamer);
                    //            std::cout << "device:" << (int)streamer.mDevice << std::endl;
                    //            std::cout << "format:" << (int)streamer.mFormat << std::endl;
                    //            std::cout << "height:" << (int)streamer.mHeight << std::endl;
                    //            std::cout << "sensor:" << (int)streamer.mSensor << std::endl;
                    //            std::cout << "width:" << (int)streamer.mWidth << std::endl;
                    //                emit w.addViewStreamSignal(streamer);
                    std::cout << "[viewer] emit addViewStreamSignal" << std::endl;
                    emit addViewStreamSignal(iStreamer);
                    //            emit addViewStreamSignal(5);
                    //            emit addViewStreamSignal();
                    //            mUi->addViewStream(streamer);
                    //                w.addViewStream(streamer);
                    break;

                case Server::Message::DEL_STREAMER: {
                    //            Streamer streamer;
                    int iStreamer;
                    sock.read(iStreamer);
                    std::cout << "[viewer] del streamer " << iStreamer << std::endl;
                    //            sock.read(streamer);
                    //            std::cout << "device:" << (int)streamer.mDevice << std::endl;
                    //            std::cout << "format:" << (int)streamer.mFormat << std::endl;
                    //            std::cout << "height:" << (int)streamer.mHeight << std::endl;
                    //            std::cout << "sensor:" << (int)streamer.mSensor << std::endl;
                    //            std::cout << "width:" << (int)streamer.mWidth << std::endl;
                    //                emit w.addViewStreamSignal(streamer);
                    //                    std::cout << "Thread_Client::run() emit addViewStreamSignal" << std::endl;
                    emit delViewStreamSignal(iStreamer);
                    //            emit addViewStreamSignal(5);
                    //            emit addViewStreamSignal();
                    //            mUi->addViewStream(streamer);
                    //                w.addViewStream(streamer);
                } break;

                default:
                    std::cout << "unknown message from server" << std::endl;
                    //                return;
                }
            }

        } catch (std::exception& e) {
            std::cout << "[viewer] catch exception : " << e.what() << std::endl;
        }

        //        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mThreadClient(this)
{

    ui->setupUi(this);

    //    mThreadClient = new Thread_Client(this);
    //    connect(mThreadClient, SIGNAL(), this, SLOT(addViewStream()));
    //    qDebug() << connect(mThreadClient, SIGNAL(), this, SLOT(addViewStream()));
    QObject::connect(&mThreadClient, &Thread_Client::addViewStreamSignal, this, &MainWindow::addViewStream);
    QObject::connect(&mThreadClient, &Thread_Client::delViewStreamSignal, this, &MainWindow::delViewStream);
    mThreadClient.start();

    //    mThreadClient.run();
}

MainWindow::~MainWindow()
{
    std::cout << "~MainWindow()" << std::endl;

    delete ui;
    std::cout << "~MainWindow() deleted ui" << std::endl;
    //    mThreadClient.terminate();
    //    mThreadClient.exit();
    //    mThreadClient.requestInterruption();
    //    mThreadClient.quit();
    mThreadClient.requestInterruption();
    mThreadClient.wait();
    //    mThreadClient.wait(1000);
    //    void requestInterruption();
    std::cout << "~MainWindow() mThreadClient.terminated()" << std::endl;
}

// void MainWindow::addViewStream(Streamer streamer)
void MainWindow::addViewStream(int iStreamer)
// void MainWindow::addViewStream()
{
    std::cout << "MainWindow::addViewStream slot" << std::endl;
    MainWindowStreamView* streamView = new MainWindowStreamView(ui->mdiArea, iStreamer);
    //    assert(mStreamId2StreamView.find(iStreamer) == mStreamId2StreamView.end());
    //    streamView->setMinimumWidth(streamer.mWidth);
    //    streamView->setMinimumHeight(streamer.mHeight);
    //    ui->mdiArea->addSubWindow(streamView);
    QMdiSubWindow* subWindow = ui->mdiArea->addSubWindow(streamView);
//    streamView->setParent(subWindow);
    //    mStreamId2StreamView[iStreamer] = streamView;
    subWindow->setVisible(true);
    //    subWindow->setParent(ui->mdiArea);
    //    streamView->setVisible(true);
    //    streamView->setEnabled(true);
    //    subWindow->setVisible(true);
    mStreamViews.insert(streamView);
    //    show();
    //    streamView->setParent(subWindow);
    //    ui->mdiArea->update();
    //    streamView->show();
}

void MainWindow::delViewStream(int iStreamer)
{
    std::cout << "MainWindow::delViewStream slot" << std::endl;

    //    assert(mStreamId2StreamView.find(iStreamer) != mStreamId2StreamView.end());
    //        QWidget_StreamView* streamView = mStreamId2StreamView.at(iStreamer);
    //        QWidget_StreamView* streamView = nullptr;
//    for (QMdiSubWindow* subWindow : ui->mdiArea->subWindowList()) {

//        int nbChild = subWindow->children().size();
//        assert(subWindow->children().size() == 1);
//        const QWidget_StreamView* streamView = static_cast<QWidget_StreamView*>(subWindow->children().first());

//        if (streamView->iStreamer() == iStreamer) {
//            ui->mdiArea->removeSubWindow(static_cast<QWidget*>(subWindow));
//            delete streamView;
//        }
//    }
    for (const auto * streamView : mStreamViews) {
        if (streamView->iStreamer() == iStreamer) {
            ui->mdiArea->removeSubWindow(streamView->parentWidget());
        }
    }

    //    QWidget_StreamView* streamView = mStreamId2StreamView.at(iStreamer);

    //    mStreamId2StreamView.erase(iStreamer);
    //    ui->mdiArea->removeSubWindow(streamView->parentWidget());

    //    delete streamView;
    //    streamView->close();

    //    ui->mdiArea->removeSubWindow(streamView);
    //    delete streamView;

    //    streamView->setMinimumWidth(streamer.mWidth);
    //    streamView->setMinimumHeight(streamer.mHeight);
    //    ui->mdiArea->addSubWindow(streamView);
    //    QMdiSubWindow* subWindow = ui->mdiArea->addSubWindow(streamView);
    //    subWindow->setVisible(true);
}
