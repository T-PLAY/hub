#ifndef HUB_H
#define HUB_H

#include <string>
#include <stream.h>

class Hub
{

public:
    Hub(std::string ipv4 = "127.0.0.1", int port = 4043);

    OutputStream createOutputStream(int width, int height, Stream::Format streamFormat, Device device = Device::NONE, Sensor sensor = Sensor::NONE);
    InputStream waitInputStream();

private:
    std::string mIpv4;
    int mPort;
//    ClientSocket mSock;
};

#endif // HUB_H
