#include "ImageStream.h"

#include <utility>
#include <iostream>

ImageStream::ImageStream(std::unique_ptr<ImageAcquisition> imAcq, int sendRate): _imAcq{std::move(imAcq)}  {
    _sendTime = 1000/sendRate;


}

ImageStream::~ImageStream() = default;

void ImageStream::streamImages() {
    _imAcq->start();

    QTcpSocket sock;

    _message = "Trying to connect";
    sock.connectToHost(_ipAddress,_port);

    if(!sock.waitForConnected(2000)) {
        _message = "Failed to connect";
        _imAcq->stop();
        _running = false;
        _error = true;
        return;
    } else {
        _connected = true;
        _message = "Connected";
    }

    _message = "Waiting for acquisition";
    while (!_imAcq->connected() && _running);
    _message = "Streaming is running";

    while (_running) {
        while (!_imAcq->imagePending() && _running);
        if (_imAcq->imagePending()) {
            const auto &image = _imAcq->image();
            int imsize = image.size();

            sock.write((const char *) &imsize, sizeof(imsize));
            std::cout << "sent size" << std::endl;
            sock.write((const char *) image.data(), imsize);
            sock.flush();
            sock.waitForBytesWritten(5000);
            _message = "Sent image of size : " + std::to_string(imsize);
            if(_running) {
                std::this_thread::sleep_for(std::chrono::milliseconds(_sendTime));
            }
        }
    }
    int imsize = -1;
    sock.write((const char *) &imsize, sizeof(imsize));
    sock.waitForBytesWritten(1000);
    std::cout << "disconnect"<< std::endl;
    sock.disconnectFromHost();
    _connected = false;
    sock.close();

}

const std::string &ImageStream::message() const {
    return _message;
}

void ImageStream::stop() {
    _running = false;
    _imAcq->stop();
    _sendThread.join();
    _message = "Stream is stopped";
}

void ImageStream::start(const QString& ipAddress) {
    _ipAddress = ipAddress;
    _running = true;
    _error = false;
    _sendThread = std::thread([this](){ImageStream::streamImages();});
}

bool ImageStream::error() const {
    return _error;
}

bool ImageStream::connected() const {
    return _connected;
}


