#include "mainwindow.h"

#include <QApplication>
#include <stream.h>
//Q_DECLARE_METATYPE(std::string)

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
//    qRegisterMetaType<Streamer>("Streamer");
    qRegisterMetaType<std::string>("std::string");
    MainWindow w;
    w.show();

//    w.mThread.start();
//    QObject::connect(&w, &MainWindow::addViewStreamSignal, &w, &MainWindow::addViewStream);

//    QObject::connect(&w.mThread, &MyThread::addViewStreamSignal, w.ui, &MainWindow::addViewStream);

//    std::thread thread([&w]() {
//        //        Hub hub("127.0.0.1", 4043);
//        ClientSocket sock("127.0.0.1", 4043);
//        sock.write(Client::Type::VIEWER);

//        while (true) {
//            Server::Message serverMessage;
//            sock.read(serverMessage);

//            switch (serverMessage) {
//            case Server::Message::NEW_STREAMER:
//                Streamer streamer;
//                sock.read(streamer);
//                std::cout << "device:" << (int)streamer.mDevice << std::endl;
//                std::cout << "format:" << (int)streamer.mFormat << std::endl;
//                std::cout << "height:" << (int)streamer.mHeight << std::endl;
//                std::cout << "sensor:" << (int)streamer.mSensor << std::endl;
//                std::cout << "width:" << (int)streamer.mWidth << std::endl;
////                emit w.addViewStreamSignal(streamer);
//                w.addViewStream(streamer);
////                w.addViewStream(streamer);
//                break;

//            default:
//                std::cout << "unknown message from server" << std::endl;
////                return;
//            }
//        }
//    });
//    thread.detach();

    return a.exec();
}
