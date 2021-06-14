#include <iostream>
#include "L515Streamer.h"

L515Streamer::L515Streamer(int readRate) : _readTime(1000/readRate) {
    _dataType = RAW;
}

std::vector<unsigned char> L515Streamer::image() {
    _pending = false;
    return _currImage;
}

bool L515Streamer::connected() const {
    return _connected;
}

bool L515Streamer::imagePending() const {
    return _pending;
}

bool L515Streamer::start() {
    if(_read) {
        std::cout << "resumed" << std::endl;
        _paused = false;
    } else {
        std::cout << "started" << std::endl;
        _paused = false;
        _read = true;
        if(_readThread.joinable())
            _readThread.join();
        _readThread = std::thread([this](){L515Streamer::readImages();});
    }

    return true;
}

void L515Streamer::stop() {
    _paused = true;
}

L515Streamer::~L515Streamer() {
    _read = false;
    if(_readThread.joinable())
        _readThread.join();
}

void L515Streamer::readImages() {
    QTcpSocket sock;
    int a = 0;
    uint imLen=0;
    sock.connectToHost("127.0.0.1",4043);
    std::cout << "trying to connect to python" << std::endl;

    if(!sock.waitForConnected(2000)) {
        std::cout << "connection timmed out" << std::endl;
        _read = false;
        return;
    } else {
        std::cout << "connected to python" << std::endl;
        _connected = true;
    }

    while(_read) {
        while (_read && _paused);
        if(_read){
            std::cout << "a : " << a << std::endl;
            sock.write(std::to_string(a++).c_str(),1);
            sock.waitForBytesWritten(1000);
            std::cout << "wrote" << std::endl;
            if(sock.waitForReadyRead(1000)) {

                std::cout << sizeof(imLen) << std::endl;
                imLen = 0;
                sock.read((char *)&imLen, sizeof(imLen));
                std::cout << "Python image lenght : " << imLen << std::endl;

                imLen = 921600;
                _currImage.clear();
                _currImage.resize(imLen);

                int totalSize = 0;
                while(totalSize != imLen) {
                    if(sock.waitForReadyRead(1000)) {
                        std::cout << "avail : " << sock.bytesAvailable() << std::endl;
                        totalSize += sock.read(reinterpret_cast<char *>(_currImage.data() + totalSize), imLen);
                        std::cout << totalSize << std::endl;
                    }
                }
                if (_read) {
                    _pending = true;
                    std::this_thread::sleep_for(std::chrono::milliseconds(_readTime));
                }
            }
        }
    }
}
