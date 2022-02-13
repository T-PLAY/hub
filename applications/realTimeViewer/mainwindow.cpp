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
    constexpr size_t imgSize = 192 * 512;
    //    constexpr size_t imgSize = 100;
    //    unsigned char* img = new unsigned char[imgSize];
    //    for (int i =0; i <imgSize; ++i) {
    //        img[i] = i * 255 / 100;
    //    }
    //    for (int i = 0; i < 512; ++i) {
    //        for (int j = 0; j < 192; ++j) {
    //            img[i * 192 + j] = j;
    //        }
    //    }
    //    ui->widget->setImage(img, 192, 512);

    std::thread thread([&]() {
        std::cout << "test" << std::endl;
        QTcpSocket sock;
        unsigned char a = 0;
        constexpr size_t imLen = 192 * 512;
        while (true) {
            sock.connectToHost("127.0.0.1", 4043);
            std::cout << "trying to connect to python" << std::endl;

            if (!sock.waitForConnected(2000)) {
                std::cout << "connection timmed out" << std::endl;
                //            _read = false;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                continue;
            } else {
                std::cout << "connected to python" << std::endl;
                //            _connected = true;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            unsigned char img[imgSize];
            ui->widget->setImage(img, 192, 512);

            while (true) {
                std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
                for (int i = 0; i < 10; ++i) {
                    std::cout << "write a : " << (int)a << std::endl;
                    //                    sock.write(std::to_string(a++).c_str(), 1);
                    int byteWrote = sock.write((char*)&a, 1);
                    if (!sock.waitForBytesWritten(1000)) {
                        std::cout << "timeout, can't write a " << std::endl;
                        return 2;
                    }
                    while (byteWrote != 1) {
                        std::cout << "couldn't write a " << byteWrote << std::endl;
                        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                        byteWrote = sock.write((char*)&a, 1);
                    }

                    //                    int byteRead = sock.read((char*)&a, 1);
                    //                    if (! sock.waitForReadyRead(1000)) {
                    //                        std::cout << "timeout, can't read a " << std::endl;
                    //                        return 2;
                    //                    }
                    //                    while (byteRead != 1) {
                    //                        std::cout << "couldn't read a " << byteRead << std::endl;
                    //                        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    //                        byteRead = sock.read((char*)&a, 1);
                    //                    }
                    //                    std::cout << "read a : " << (int)a << std::endl;
                    ++a;

                    //                    sock.waitForBytesWritten(1000);
                    ////                    std::cout << "wrote" << std::endl;
                    //                    if (sock.waitForReadyRead(5000)) {
                    //                imLen = 0;
                    //                sock.read((char*)&imLen, sizeof(imLen));
                    //                std::cout << "Python image lenght : " << imLen << std::endl;

                    //                _currImage.clear();
                    //                _currImage.resize(imLen);

                    int totalSize = 0;
                    while (totalSize != imLen) {
                        //                            if (sock.waitForReadyRead(1000)) {
                        //                                std::cout << "avail : " << sock.bytesAvailable() << std::endl;
                        //                                std::cout << "write : " << sock.bytesToWrite() << std::endl;
                        //                        totalSize += sock.read(reinterpret_cast<char*>(_currImage.data() + totalSize), imLen);
                        //                            totalSize += sock.read(reinterpret_cast<char*>(img + totalSize), imLen);
                        if (!sock.waitForReadyRead(1000)) {
                            std::cout << "timeout, can't read img " << std::endl;
                            return 2;
                        }
                        totalSize += sock.read((char*)img + totalSize, imLen);
                        std::cout << "read " << totalSize << " bytes" << std::endl;
                        //                            }
                    }
                    //                    ui->widget->setImage(img, 192, 512);
                    ui->widget->update();
                    //                    }
                    //                    else {
                    //                        std::cout << "couldn't read, timeout" << std::endl;
                    //                        return 1;
                    //                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
                std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
                const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                double fps = 10.0 * 1'000 / duration;
                //                std::cout << "fps : " << fps << std::endl;
                ui->statusbar->showMessage((std::string("fps:") + std::to_string(fps)).c_str());
            }
        }
    });
    thread.detach();
}

MainWindow::~MainWindow()
{
    delete ui;
}
