#include "stream.h"

#include <socket.h>


Stream::Stream(int width, int height, Format format, Device device, Sensor sensor, int port, std::string ipv4)
    //    : mType(Type::NONE)
    : mInitPacket{width, height, format, device, sensor}
//    : mWidth(width)
//    , mHeight(height)
//    , mFormat(format)
//    , mDevice(device)
//    , mSensor(sensor)
    , mSocket(ipv4, port)
    , mAcquisitionSize(width * height * formatNbByte[static_cast<int>(format)])
{
    //    mSocket.sendData(InitPacket{width, height, mFormat, mDevice, mSensor});
}

InputStream::InputStream(int width, int height, Format format, Device device, Sensor sensor, int port, const std::string& ipv4)
    : Stream(width, height, format, device, sensor, port, ipv4)
{
}

OutputStream::OutputStream(int width, int height, Stream::Format format, Device device, Sensor sensor, int port, const std::string& ipv4)
    : Stream(width, height, format, device, sensor, port, ipv4)
{
    //    mType = Type::OUTPUT;
    Client::Type clientType = Client::Type::STREAMER;
    mSocket.write(clientType);

//    mSocket.write((char*)&mWidth, sizeof(InitPacket));
    mSocket.write(mInitPacket);
}

void OutputStream::operator<<(const Acquisition& acquisition)
{
    //    mSocket.sendData(packet);
//    mSocket.write((char*)&acquisition.backendTimestamp, sizeof(acquisition) - sizeof(acquisition.data));
    mSocket.write(acquisition.timestampInterval);
    mSocket.write(acquisition.data, mAcquisitionSize);
}
