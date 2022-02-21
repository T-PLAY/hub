#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <thread>

//#include <QTcpSocket>
//#include <iostream>
#include <QDebug>
#include <QMdiSubWindow>
#include <QStandardItemModel>
#include <hub.h>
#include <qwidget_streamview.h>
#include <server.h>

Thread_Client::Thread_Client(QObject *parent)
    :QThread(parent)
{
    std::cout << "Thread_Client()" << std::endl;
//    QObject::connect(this, &Thread_Client::addViewStreamSignal, mainWindow, &MainWindow::addViewStream);
}

void Thread_Client::run()
{
    std::cout << "Thread_Client::run()" << std::endl;
    //        Hub hub("127.0.0.1", 4043);
    ClientSocket sock("127.0.0.1", 4043);
    sock.write(Client::Type::VIEWER);

    while (true) {
        Server::Message serverMessage;
        sock.read(serverMessage);

        switch (serverMessage) {
        case Server::Message::NEW_STREAMER:
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
            std::cout << "Thread_Client::run() emit addViewStreamSignal" << std::endl;
            emit addViewStreamSignal(iStreamer);
//            emit addViewStreamSignal(5);
//            emit addViewStreamSignal();
            //            mUi->addViewStream(streamer);
            //                w.addViewStream(streamer);
            break;

        default:
            std::cout << "unknown message from server" << std::endl;
            //                return;
        }
    }
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    mThreadClient = new Thread_Client(this);
//    connect(mThreadClient, SIGNAL(), this, SLOT(addViewStream()));
//    qDebug() << connect(mThreadClient, SIGNAL(), this, SLOT(addViewStream()));
    QObject::connect(mThreadClient, &Thread_Client::addViewStreamSignal, this, &MainWindow::addViewStream);
    mThreadClient->start();

}

// void MainWindow::routine()
//{

//    static const size_t imgSize = 192 * 512;
//    QTreeView* treeView = ui->treeView;
//    QStandardItemModel* model = new QStandardItemModel();
//    treeView->setModel(model);

//    QStandardItem* parentItem = model->invisibleRootItem();
//    //    parentItem->setText("salut");

//    Hub hub("127.0.0.1", 4043);

//    //    QWidget_StreamView* streamView = new QWidget_StreamView();
//    //    streamView.setStyleSheet("background-color: green");
//    //    ui->mdiArea->addSubWindow(streamView);

//    //    ClientSocket sock("127.0.0.1", 4043);
//    //    Client::Type clientType = Client::Type::VIEWER;
//    //    sock.write(clientType);

//    //    QWidget_StreamView* streamView = new QWidget_StreamView(ui->mdiArea);
//    //    streamView->setMinimumWidth(100);
//    //    streamView->setMinimumHeight(200);
//    //    ui->mdiArea->addSubWindow(streamView);
////    QMdiArea* mdiArea = ui->mdiArea;
//    QMdiArea* mdiArea = new QMdiArea(this);
//    ui->mdiArea = mdiArea;
//    //        ui->mdiArea->addSubWindow(streamView);
//    mdiArea->setOption(QMdiArea::DontMaximizeSubWindowOnActivation, false);
//    //    setCentralWidget(mdiArea);
//    std::cout << ui << std::endl;

//    //    std::thread thread([&, sock = std::move(sock)]() {
//    std::thread thread([=]() {
//        std::cout << ui << std::endl;
//        QWidget* streamView = new QWidget(mdiArea, Qt::FramelessWindowHint);
//        streamView->setMinimumWidth(512);
//        streamView->setMinimumHeight(192);
//        //        QWidget_StreamView* streamView = new QWidget_StreamView();
//        //        streamView->show();
//        //        QMdiSubWindow * subWindow = ui->mdiArea->addSubWindow(streamView);
//        QMdiSubWindow* subWindow = new QMdiSubWindow();
//        subWindow->setParent(mdiArea);
//        //        subWindow->update();
//        subWindow->setWidget(streamView);
//        subWindow->show();
//        //        qDebug() << subWindow;
//        //        std::cout << subWindow << std::endl;
//        //        subWindow->setWindowState(Qt::WindowMaximized);
//        //        assert(subWindow != nullptr);
//        //        ui->mdiArea->setActiveSubWindow(subWindow);
//        //        subWindow->
//        //                streamView->show();
//        //        ui->mdiArea->setActiveSubWindow(streamView)

//        //        while (true) {
//        //            Server::Message message;
//        //            sock.read(message);
//        //            switch (message) {
//        //            case Server::Message::LIST_OF_STREAMERS: {
//        //                std::cout << "list of streamers" << std::endl;
//        //                std::list<Streamer> streamers;
//        //                sock.read(streamers);
//        //                for (const auto& streamer : streamers) {
//        //                    std::cout << "streamer " << streamer.mWidth << " " << streamer.mHeight << std::endl;

//        //                    QStandardItem* item = new QStandardItem(device2string[static_cast<int>(streamer.mDevice)].c_str());
//        //                    parentItem->appendRow(item);

//        //                    QWidget_StreamView* streamView = new QWidget_StreamView(ui->mdiArea);
//        //                    //                    streamView->setMinimumWidth(streamer.mWidth);
//        //                    //                    streamView->setMinimumHeight(streamer.mHeight);
//        //                    ui->mdiArea->addSubWindow(streamView);
//        //                }
//        //            } break;

//        //            default:
//        //                std::cout << "unknown message from server" << std::endl;
//        //                exit(2);
//        //            }
//        //        }
//    });
//    thread.detach();
//    //    thread.join();

//    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//    Client::Message clientMessage = Client::Message::GET_CONNECTED_DEVICES;
//    sock.write(clientMessage);

//    std::thread thread([&]() {
//        QTcpSocket sock;
//        unsigned char a = 0;

//        // for each connection to server
//        while (true) {
//            sock.connectToHost("127.0.0.1", 4043);
//            std::cout << "trying to connect to server" << std::endl;

//            if (!sock.waitForConnected(2000)) {
//#ifndef WIN32
//                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
//#endif
//                std::cout << "connection time out" << std::endl;
//                continue;
//            }

//            unsigned char img[imgSize];
////            ui->widget->setImage(img, 192, 512);
//            ui->widget->setImage(img, 512, 192);

//            bool isConnected = true;
//            while (isConnected) {

//                std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
//                for (int i = 0; i < 10; ++i) {
//                    std::cout << "write a : " << (int)a << std::endl;
//                    int byteWrote = sock.write((char*)&a, 1);
//                    if (!sock.waitForBytesWritten(1000)) {
//                        std::cout << "timeout, can't write a " << std::endl;
//                        isConnected = false;
//                        ui->widget->setImage(nullptr, 0, 0);
//                        break;
//                    }
//                    while (byteWrote != 1) {
//                        std::cout << "couldn't write a " << byteWrote << std::endl;
//                        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//                        byteWrote = sock.write((char*)&a, 1);
//                    }

//                    ++a;

//                    int totalSize = 0;
//                    while (totalSize != imgSize) {

//                        if (!sock.waitForReadyRead(1000)) {
//                            std::cout << "timeout, can't read img " << std::endl;
//                            exit(1);
//                        }
//                        totalSize += sock.read((char*)img + totalSize, imgSize);
//                        std::cout << "read " << totalSize << " bytes" << std::endl;
//                    }
//                    ui->widget->update();

//                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
//                }

//                if (isConnected) {
//                    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
//                    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//                    double fps = 10.0 * 1'000 / duration;
//                                        //                std::cout << "fps : " << fps << std::endl;
//                                        ui->statusbar->showMessage((std::string("fps:") + std::to_string(fps)).c_str());
//                }
//                else {
//                    ui->statusbar->clearMessage();
//                }
//            }
//        }
//    });
//    thread.detach();
//}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::addViewStream(Streamer streamer)
void MainWindow::addViewStream(int iStreamer)
//void MainWindow::addViewStream()
{
    std::cout << "MainWindow::addViewStream slot" << std::endl;
    QWidget_StreamView* streamView = new QWidget_StreamView(ui->mdiArea, iStreamer);
//    streamView->setMinimumWidth(streamer.mWidth);
//    streamView->setMinimumHeight(streamer.mHeight);
    //    ui->mdiArea->addSubWindow(streamView);
    QMdiSubWindow* subWindow = ui->mdiArea->addSubWindow(streamView);
    subWindow->setVisible(true);
//    subWindow->setParent(ui->mdiArea);
//    streamView->setVisible(true);
//    streamView->setEnabled(true);
    //    subWindow->setVisible(true);
    //    show();
    //    streamView->setParent(subWindow);
    //    ui->mdiArea->update();
    //    streamView->show();
}
