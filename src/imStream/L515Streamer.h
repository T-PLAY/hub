#ifndef HUBV2_L515STREAMER_H
#define HUBV2_L515STREAMER_H

#include "ImageAcquisition.h"
#include <vector>
#include <QTcpSocket>
#include <thread>

class L515Streamer : public ImageAcquisition {
public :
    explicit L515Streamer(int readRate);

    std::vector<unsigned char> image() final;
    bool connected() const final;
    bool imagePending() const final;
    bool start() final;
    void stop() final;

    ~L515Streamer();

private :
    std::vector<unsigned char> _currImage;
    bool _connected = false;
    bool _pending = false;
    bool _read = false;
    bool _paused = false;

    std::thread _readThread;
    int _readTime;

    void readImages();
};

#endif //HUBV2_L515STREAMER_H
