#ifndef HUBV2_FILESTREAMER_H
#define HUBV2_FILESTREAMER_H

#include <vector>
#include <thread>
#include <QDir>
#include <random>
#include <ctime>
#include <fstream>

#include "ImageAcquisition.h"

class FileStreamer : public ImageAcquisition {
public :
    FileStreamer(const QString& folder, int readRate);

    std::vector<unsigned char> image() final;
    bool connected() const final;
    bool imagePending() const final;
    void start() final;
    void stop() final;

private :
    std::vector<unsigned char> _currImage;
    bool _connected = false;
    bool _pending = false;
    bool _read = false;

    std::thread _readThread;
    int _readTime;

    QDir _dir;
    QStringList _list;

    void readImages();

};

#endif
