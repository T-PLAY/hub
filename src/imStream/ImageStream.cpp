#include "ImageStream.h"

#include <utility>

ImageStream::ImageStream(std::unique_ptr<ImageAcquisition> imAcq, QString& ipAddress, int sendRate): _ipAddress{ipAddress}, _sendRate{sendRate}, _imAcq{std::move(imAcq)}  {

}

ImageStream::~ImageStream() {
    _sock.close();
}

void ImageStream::streamImages() {
    _message = "Waiting for acquisition";
    while (!_imAcq->connected());
    _message = "Trying to connect";
    _sock.connectToHost(_ipAddress,_port);
    

    while (_running) {




    }


}

const std::string &ImageStream::message() const {
    return _message;
}


