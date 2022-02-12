#include <iostream>
#include "BasicTracker.h"

bool BasicTracker::connected() const {
    return _connected;
}

TrackerData BasicTracker::data() const {
    return _data;
}

void BasicTracker::start() {
    _sock.bind(5123);

    _readThread = std::thread([this](){BasicTracker::readData();});


}

void BasicTracker::end() {
    _listen = false;
    _readThread.join();

    _sock.close();
}

void BasicTracker::readData() {

    std::cout << "read" << std::endl;
    char buffer[256];
    long long nread = 0;
    while(_listen) {
        nread = _sock.readDatagram(buffer,256);

        _connected = true;

        int offset = 0;
        while(offset + 33 <= nread) {
            _data.nId = (buffer[offset+2]) - 1;
            offset += 12;

            _data.v =  *reinterpret_cast<float*>(buffer+offset)/div;
            _data.t =  *reinterpret_cast<float *>(buffer+offset+4)/div;
            _data.u = - *reinterpret_cast<float *>(buffer+offset+8)/div;
            offset += 12;

            _data.w = *reinterpret_cast<float *>(buffer+offset);
            _data.z = -*reinterpret_cast<float *>(buffer+offset+4);
            _data.x = -*reinterpret_cast<float *>(buffer+offset+8);
            _data.y = *reinterpret_cast<float *>(buffer+offset+12);
            offset += 16;

        }

    }
    std::cout << "end read" << std::endl;
}


