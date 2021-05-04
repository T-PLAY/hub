#ifndef HUBV2_IMAGESTREAM_H
#define HUBV2_IMAGESTREAM_H

#include <QTcpSocket>
#include <memory>
#include <thread>
#include "ImageAcquisition.h"

class ImageStream {
private :
    QTcpSocket _sock;
    QString _ipAddress;

    const int _port = 4041;
    int _sendRate;

    bool _paused = false;
    bool _running = false;

    std::string _message;

    std::unique_ptr<ImageAcquisition> _imAcq;

    void streamImages();

public:
    ImageStream(std::unique_ptr<ImageAcquisition> imAcq, QString& ipAddress, int sendRate);

    void start();
    void stop();

    const std::string& message() const;

    ~ImageStream();

};


#endif
