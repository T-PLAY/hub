#include "stream.h"

#include <hub.h>


Stream::Stream(int width, int height, Format format, Device device, Sensor sensor, int port, std::string ipv4)
    : mStreamInit{width, height, format, device, sensor}
//    , mPort(port)
//    , mIpv4(ipv4)
    , mHub(ipv4, port)
{


    mHub.sendPacket(mStreamInit);

}
