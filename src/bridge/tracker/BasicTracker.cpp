#include <iostream>
#include "BasicTracker.h"

bool BasicTracker::connected() const {
    return _connected;
}

TrackerData BasicTracker::data() const {
    return _data;
}

void BasicTracker::start() {
#ifdef _WIN32
    bind(_sock,(SOCKADDR *)&_addr,sizeof(_addr));
#elif __linux__
    bind(_sock , (struct sockaddr*)&_addr, sizeof(_addr));
#endif

    _readThread = std::thread([this](){BasicTracker::readData();});


}

void BasicTracker::end() {
    _listen = false;
    _readThread.join();

#ifdef _WIN32
    closesocket(_sock);
    WSACleanup();
#elif __linux__
    close(_sock);
#endif
}

BasicTracker::BasicTracker() {
#ifdef _WIN32
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2,0), &WSAData);

    _addr.sin_addr.s_addr = htonl (INADDR_ANY);
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(5123);
    _sock = socket(AF_INET,SOCK_DGRAM,0);
#elif __linux__
    _sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    memset((char *) &_addr, 0, sizeof(_addr));

    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(5123);
    _addr.sin_addr.s_addr = htonl(INADDR_ANY);

#endif
}

void BasicTracker::readData() {

    struct sockaddr udpaddrfrom;
#ifdef _WIN32
    int fromlen = sizeof(udpaddrfrom);
#elif __linux__
    socklen_t fromlen = sizeof(udpaddrfrom);
#endif

    char buffer[256];
    int nread = 0;
    while(_listen) {
        nread = recvfrom(_sock,buffer,sizeof(buffer),0,&udpaddrfrom,&fromlen);
        _connected = true;

        int offset = 0;
        while(offset + 33 <= nread) {
            _data.nId = (buffer[offset+2]) - 1;
            offset += 12;

            _data.v = - *reinterpret_cast<float*>(buffer+offset)/div;
            _data.t = - *reinterpret_cast<float *>(buffer+offset+4)/div;
            _data.u = *reinterpret_cast<float *>(buffer+offset+8)/div;
            offset += 12;

            _data.w = *reinterpret_cast<float *>(buffer+offset);
            _data.z = -*reinterpret_cast<float *>(buffer+offset+4);
            _data.x = -*reinterpret_cast<float *>(buffer+offset+8);
            _data.y = *reinterpret_cast<float *>(buffer+offset+12);
            offset += 16;

        }

    }
}


