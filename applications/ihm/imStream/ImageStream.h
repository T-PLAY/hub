#ifndef HUBV2_IMAGESTREAM_H
#define HUBV2_IMAGESTREAM_H

#include <QTcpSocket>
#include <memory>
#include <thread>
#include "ImageAcquisition.h"

class ImageStream {
private :
    QString _ipAddress;

    const int _port = 4041;
    int _sendTime;

    bool _paused = false;
    bool _running = false;
    bool _connected = false;
    bool _error = false;

    std::string _message;

    std::unique_ptr<ImageAcquisition> _imAcq;
    std::thread _sendThread;

    void streamImages();

public:
    ImageStream(std::unique_ptr<ImageAcquisition> imAcq, int sendRate);

    void start(const QString& ipAddress);
    void stop();

    bool error() const;
    bool connected() const;

    std::vector<unsigned char> getCurrentImage() const;

    const std::string& message() const;

    ~ImageStream();

};


#endif
