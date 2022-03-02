#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMdiSubWindow>
#include <QStandardItemModel>
#include <hub.h>
#include <server.h>

Thread_Client::Thread_Client(QObject* parent)
    : QThread(parent)
{
    std::cout << "Thread_Client()" << std::endl;
}

void Thread_Client::run()
{
    std::cout << "Thread_Client::run()" << std::endl;

    while (!this->isInterruptionRequested()) {
        try {
            ClientSocket sock("127.0.0.1");
            sock.write(Client::Type::VIEWER);

            while (!this->isInterruptionRequested()) {

                Socket::Message serverMessage;
                sock.read(serverMessage);

                switch (serverMessage) {

                case Socket::Message::PING: {
                    // server check client connection
                    // nothing to do
                    //                    std::cout << "Thread_Client::run() server ping" << std::endl;
                } break;

                case Socket::Message::NEW_STREAMER: {
                    std::cout << "[viewer] new streamer" << std::endl;
                    std::string streamerSensorName;
                    sock.read(streamerSensorName);

                    std::cout << "[viewer] emit addViewStreamSignal '" << streamerSensorName << "'" << std::endl;
                    emit addViewStreamSignal(streamerSensorName);
                } break;

                case Socket::Message::DEL_STREAMER: {
                    std::string streamerSensorName;
                    sock.read(streamerSensorName);
                    std::cout << "[viewer] del streamer '" << streamerSensorName << "'" << std::endl;
                    emit delViewStreamSignal(streamerSensorName);
                } break;

                default:
                    std::cout << "unknown message from server" << std::endl;
                }
            }

            //            sock.write(Socket::Message::CLOSE);
            //            Socket::Message message;
            //            sock.read(message);
            //            assert(message == Socket::Message::OK);

        } catch (std::exception& e) {
            std::cout << "[viewer] catch exception : " << e.what() << std::endl;
        }
    } // while (!this->isInterruptionRequested())
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mThreadClient(this)
{

    ui->setupUi(this);

    QObject::connect(&mThreadClient, &Thread_Client::addViewStreamSignal, this, &MainWindow::addViewStream);
    QObject::connect(&mThreadClient, &Thread_Client::delViewStreamSignal, this, &MainWindow::delViewStream);
    mThreadClient.start();
}

MainWindow::~MainWindow()
{
    std::cout << "~MainWindow()" << std::endl;

    delete ui;
    std::cout << "~MainWindow() deleted ui" << std::endl;

    mThreadClient.requestInterruption();
    mThreadClient.wait();

    std::cout << "~MainWindow() mThreadClient.terminated()" << std::endl;
}

void MainWindow::addViewStream(std::string streamerSensorName)
{
    std::cout << "MainWindow::addViewStream slot '" << streamerSensorName << "'" << std::endl;
    MainWindowStreamView* streamView = new MainWindowStreamView(ui->mdiArea, streamerSensorName);

    QMdiSubWindow* subWindow = ui->mdiArea->addSubWindow(streamView);
    subWindow->setVisible(true);
    mStreamViews.insert(streamView);
}

void MainWindow::delViewStream(std::string streamerSensorName)
{
    std::cout << "MainWindow::delViewStream slot" << std::endl;

    for (auto* streamView : mStreamViews) {
        if (streamView->getStreamerSensorName() == streamerSensorName) {
            ui->mdiArea->removeSubWindow(streamView->parentWidget());
            mStreamViews.erase(streamView);
            //            delete streamView;
        }
    }
}
