#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <thread>

#include <QTcpSocket>
#include <iostream>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    static const size_t imgSize = 192 * 512;

    std::thread thread([&]() {
        QTcpSocket sock;
        unsigned char a = 0;

        // for each connection to server
        while (true) {
            sock.connectToHost("127.0.0.1", 4043);
            std::cout << "trying to connect to server" << std::endl;

            if (!sock.waitForConnected(2000)) {
                std::cout << "connection time out" << std::endl;
                continue;
            }

            unsigned char img[imgSize];
            ui->widget->setImage(img, 192, 512);

            bool isConnected = true;
            while (isConnected) {

                std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
                for (int i = 0; i < 10; ++i) {
                    std::cout << "write a : " << (int)a << std::endl;
                    int byteWrote = sock.write((char*)&a, 1);
                    if (!sock.waitForBytesWritten(1000)) {
                        std::cout << "timeout, can't write a " << std::endl;
                        isConnected = false;
                        ui->widget->setImage(nullptr, 0, 0);
                        break;
                    }
                    while (byteWrote != 1) {
                        std::cout << "couldn't write a " << byteWrote << std::endl;
                        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                        byteWrote = sock.write((char*)&a, 1);
                    }

                    ++a;

                    int totalSize = 0;
                    while (totalSize != imgSize) {

                        if (!sock.waitForReadyRead(1000)) {
                            std::cout << "timeout, can't read img " << std::endl;
                            exit(1);
                        }
                        totalSize += sock.read((char*)img + totalSize, imgSize);
                        std::cout << "read " << totalSize << " bytes" << std::endl;
                    }
                    ui->widget->update();

                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }

                if (isConnected) {
                    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
                    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                    double fps = 10.0 * 1'000 / duration;
                                        //                std::cout << "fps : " << fps << std::endl;
                                        ui->statusbar->showMessage((std::string("fps:") + std::to_string(fps)).c_str());
                }
                else {
                    ui->statusbar->clearMessage();
                }
            }
        }
    });
    thread.detach();
}

MainWindow::~MainWindow()
{
    delete ui;
}
