#include "stream.h"

#include <cassert>
#include <functional>
#include <numeric>
//#include <server.h>
#include <socket.h>

// Stream::Stream(int width, int height, Format format, Device device, Sensor sensor, int port, std::string ipv4)
//     //    : mType(Type::NONE)
////    : mInitPacket{width, height, format, device, sensor}
////    : mWidth(width)
////    , mHeight(height)
////    , mFormat(format)
////    , mDevice(device)
////    , mSensor(sensor)
//    , mSocket(ipv4, port)
//    , mAcquisitionSize(width * height * formatNbByte[static_cast<int>(format)])
//{
//    //    mSocket.sendData(InitPacket{width, height, mFormat, mDevice, mSensor});
//}

Stream::Stream(const std::string& sensorName, Format format, const std::vector<int>& dims, const std::string& ipv4, int port)
    : mSensorName(sensorName)
    , mFormat(format)
    , mDims(dims)
    , mSocket(ipv4, port)
    , mAcquisitionSize(computeAcquisitionSize(format, dims))
//    , mAcquisitionSize(std::accumulate(dims.cbegin(), dims.cend(), 1, std::multiplies<int> {}) * formatNbByte[static_cast<int>(format)])
//    , mAcquisitionSize(width * height * formatNbByte[static_cast<int>(format)])
{
#ifdef DEBUG_MSG
    std::cout << "[Stream] Stream()" << std::endl;
#endif
    //    setupAcquisitionSize();
}

Stream::~Stream()
{
#ifdef DEBUG_MSG
    std::cout << "[Stream] ~Stream()" << std::endl;
#endif
}

Stream::Stream(ClientSocket&& clientSocket)
    : mSocket(std::move(clientSocket))
{
}

// void Stream::setupAcquisitionSize()
//{
//     mAcquisitionSize = std::accumulate(mDims.cbegin(), mDims.cend(), 1, std::multiplies<int> {}) * formatNbByte[static_cast<int>(mFormat)];
// }

size_t Stream::computeAcquisitionSize(Format format, const std::vector<int>& dims)
{
    return std::accumulate(dims.cbegin(), dims.cend(), 1, std::multiplies<int> {}) * formatNbByte[static_cast<int>(format)];
}

void Stream::ping() const
{
    mSocket.write(Socket::Message::PING);
}

void Stream::close()
{
    mSocket.write(Socket::Message::CLOSE);
}

void Stream::waitClose()
{
    while (true) {
        Socket::Message clientMessage;
        mSocket.read(clientMessage);

        std::cout << "####### read message " << Socket::message2string[(int)clientMessage] << " from stream viewer" << std::endl;

        if (clientMessage == Socket::Message::CLOSE) {
            std::cout << "####### stream viewer want to close" << std::endl;
            break;
        }
    }
}

const std::string& Stream::getSensorName() const
{
    return mSensorName;
}

const std::vector<int>& Stream::getDims() const
{
    return mDims;
}

size_t Stream::getAcquisitionSize() const
{
    //    return std::accumulate(mDims.cbegin(), mDims.cend(), 1, std::multiplies<int> {}) * formatNbByte[static_cast<int>(mFormat)];
    return mAcquisitionSize;
}

Stream::Format Stream::getFormat() const
{
    return mFormat;
}

///////////////////////////////////////////////////////////////////////////////

InputStream::InputStream(const std::string& sensorName, const std::string& ipv4, int port)
    : Stream(sensorName, Format::NONE, {}, ipv4, port)
{
#ifdef DEBUG_MSG
    std::cout << "[InputStream] InputStream(sensorName, ipv4, port)" << std::endl;
#endif
    Client::Type clientType = Client::Type::STREAM_VIEWER;
    mSocket.write(clientType);

    mSocket.write(sensorName);
    //    mSocket.write(sensorName);

    //    mSocket.read(mSensorName);
    mSocket.read(mFormat);
    mSocket.read(mDims);

    //    mAcquisitionSize = getAcquisitionSize();
    mAcquisitionSize = computeAcquisitionSize(mFormat, mDims);
}

InputStream::InputStream(ClientSocket&& sock)
    : Stream(std::move(sock))
{
#ifdef DEBUG_MSG
    std::cout << "[InputStream] InputStream(ClientSocket && sock)" << std::endl;
#endif
    mSocket.read(mSensorName);
    mSocket.read(mFormat);
    mSocket.read(mDims);

    mAcquisitionSize = computeAcquisitionSize(mFormat, mDims);
}

InputStream::~InputStream()
{
#ifdef DEBUG_MSG
    std::cout << "[InputStream] ~InputStream()" << std::endl;
#endif

    //    if (this->isInterruptionRequested() && !serverRequestClose) {
    //    mSocket.write(Socket::Message::CLOSE);
    //    Socket::Message serverMessage;
    //    mSocket.read(serverMessage);
    //    assert(serverMessage == Socket::Message::OK);
    //    }
}

void InputStream::operator>>(Acquisition& acquisition) const
{
    std::cout << "[InputStream] operator>>(acq)" << std::endl;

    //    while (!this->isInterruptionRequested() && !serverRequestClose) {
    mSocket.write(Socket::Message::SYNC);
    std::cout << "[InputStream] wrote sync" << std::endl;
    Socket::Message message;
    mSocket.read(message);
    std::cout << "[InputStream] read message" << std::endl;

    switch (message) {
    case Socket::Message::DATA: {
        //        Stream::TimestampInterval timestampInterval;
        std::cout << "[InputStream] read data" << std::endl;
        //        mSocket.read(timestampInterval);
        mSocket.read(acquisition.backendTimestamp);
        mSocket.read(acquisition.backendTimeOfArrival);
        mSocket.read(acquisition.data, mAcquisitionSize);

    } break;
    case Socket::Message::CLOSE:
        std::cout << "[InputStream] request close" << std::endl;
        //        serverRequestClose = true;
        throw stream_exception();
        break;

    case Socket::Message::PING:
        std::cout << "[InputStream] request ping" << std::endl;
        break;

    default:
        std::cout << "unknown message from server" << std::endl;
        exit(1);
    }

    //    sock.write(Message::SYNC);
    //    std::cout << getServerHeader(iThread) << "send sync start new acquisition\t server status : " << getStatus() << std::endl;

    //    Client::Message message;
    //    sock.read(message);

    //    assert(message == Client::Message::DATA);

    //    //                            switch (message) {
    //    //                            case Client::Message::PING:
    //    //                                std::cout << getServerHeader(iThread) << "client is pinging" << std::endl;
    //    //                                break;

    //    //                            case Client::Message::DATA:
    //    //                    char sync = 's';
    //    //                    sock.write(sync);

    //    //                            Stream::TimestampInterval timestampInterval;
    //    //                            sock.read(timestampInterval);
    //    Stream::Acquisition acq;
    //    sock.write(acq.backendTimestamp);
    //    sock.write(acq.backendTimeOfArrival);
    //    sock.write(acq.data, acquistionSize);
    //    //                    sock.read()
    //    sock.read(data, acquistionSize);
    //    //    }
    std::cout << "[InputStream] operator>>(acq) end" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////

OutputStream::OutputStream(const std::string& sensorName, Stream::Format format, const std::vector<int>& dims, const std::string& ipv4, int port)
    : Stream(sensorName, format, dims, ipv4, port)
{
    //    mType = Type::OUTPUT;
    Client::Type clientType = Client::Type::STREAMER;
    mSocket.write(clientType);

    mSocket.write(mSensorName);
    mSocket.write(mFormat);
    mSocket.write(mDims);

    //    mSocket.write((char*)&mWidth, sizeof(InitPacket));
    //    mSocket.write(mInitPacket);
}

OutputStream::OutputStream(ClientSocket&& sock, const InputStream& inputStream)
    : Stream(std::move(sock))
{
    mSensorName = inputStream.getSensorName();
    mFormat = inputStream.getFormat();
    mDims = inputStream.getDims();

    mAcquisitionSize = computeAcquisitionSize(mFormat, mDims);

    mSocket.write(mFormat);
    mSocket.write(mDims);
}

void OutputStream::operator<<(const Acquisition& acquisition)
{
    std::cout << "[OutputStream] operator<<(const acq)" << std::endl;

    //    mSocket.sendData(packet);
    //    mSocket.write((char*)&acquisition.backendTimestamp, sizeof(acquisition) - sizeof(acquisition.data));
    //    mSocket.write(acquisition.timestampInterval);
    //    mSocket.write(acquisition.data, mAcquisitionSize);

    Socket::Message message;

    bool acquisitionSent = false;
    while (!acquisitionSent) {

        mSocket.read(message);

        switch (message) {
        case Socket::Message::PING:
            std::cout << "[OutputStream] request ping" << std::endl;
            break;

        case Socket::Message::CLOSE:
            std::cout << "[OutputStream] request close" << std::endl;
            //        serverClose = true;
            break;

        case Socket::Message::SYNC: {
            //        std::cout << "[OutputStream] server sync, dec = " << dec << std::endl;
            std::cout << "[OutputStream] request data" << std::endl;

            mSocket.write(Socket::Message::DATA);
            //                char sync;
            //                mSocket.read(sync);
            //                assert(sync == 's');
            //            unsigned char a;

            //            int byteRead = recv(new_socket, (char*)&a, 1, 0);
            //            if (byteRead != 1) {
            //                std::cout << "can't read sync byte " << byteRead << std::endl;
            //                break;
            //            }
            //            std::cout << "read a : " << (int)a << std::endl;

            //            int byteSent = send(new_socket, (char*)img, imgSize, 0);
            //            if (byteSent != imgSize) {
            //                std::cout << "can't send sync byte " << byteSent << std::endl;
            //                break;
            //            }

            //            std::cout << "sent " << byteSent << " bytes" << std::endl;
            //            proceduralStream << Stream::Acquisition{{start, end}, (char*)img};
            mSocket.write(acquisition.backendTimestamp);
            mSocket.write(acquisition.backendTimeOfArrival);
            mSocket.write(acquisition.data, mAcquisitionSize);

            std::cout << "[OutputStream] wrote data" << std::endl;
            //                    constexpr int width = 192;
            //                    constexpr int height = 512;
            //                    int dec = img[0];
            //                    for (int i = 0; i < width; ++i) {
            //                        for (int j = 0; j < height; ++j) {
            //                            assert(img[i + j * width] == (j + dec) % 256);
            //                        }
            acquisitionSent = true;
            //                    }
        } break;

        default:
            std::cout << "unknown message from server" << std::endl;
            exit(1);
        }
    }
    //    streamViewer->socket->write(Socket::Message::DATA);
    //    //                                streamViewer->socket->write(timestampInterval);
    //    streamViewer->socket->write(acq.backendTimestamp);
    //    streamViewer->socket->write(acq.backendTimeOfArrival);
    //    streamViewer->socket->write(data, acquistionSize);
    std::cout << "[OutputStream] operator<<(const acq) end" << std::endl;
}
