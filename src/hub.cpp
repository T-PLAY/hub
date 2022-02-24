#include "hub.h"

#include <socket.h>

Hub::Hub(std::string ipv4, int port)
    : mIpv4(ipv4)
    , mPort(port)
//    , mSock(ipv4, port)
{
}

//OutputStream Hub::createOutputStream(int width, int height, Stream::Format streamFormat, Device device, Sensor sensor)
//{
//    OutputStream outputStream(width, height, streamFormat, device, sensor, mPort, mIpv4);

//    return outputStream;
//}

InputStream Hub::waitInputStream()
{

}
