#include "stream.h"

#include <cassert>
#include <functional>
#include <numeric>
#include <socket.h>
//#include <math.h>

std::ostream& operator<<(std::ostream& os, const Stream::Acquisition& acq)
{
    os << "start:" << acq.backendTimestamp / 1000 << ", end:" << acq.backendTimeOfArrival / 1000 << ", size:" << acq.acquisitionSize;
    os << ", data:[";
    for (int i = 0; i < min(10, acq.acquisitionSize); ++i) {
        os << (int)acq.data[i] << " ";
    }
    os << "]";
    return os;
}

Stream::Acquisition Stream::acquisition() const
{
    Acquisition acq { 0, 0, mAcquisitionSize, new unsigned char[mAcquisitionSize] };
    for (size_t i = 0; i < mAcquisitionSize; ++i) {
        acq.data[i] = 0;
    }
    return acq;
}

Stream::Stream(const std::string& sensorName, Format format, const std::vector<int>& dims, const std::string& ipv4, int port)
    : mSensorName(sensorName)
    , mFormat(format)
    , mDims(dims)
    , mSocket(ipv4, port)
    , mAcquisitionSize(computeAcquisitionSize(format, dims))
{
#ifdef DEBUG_MSG
    std::cout << "[Stream] Stream()" << std::endl;
#endif
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

size_t Stream::computeAcquisitionSize(Format format, const std::vector<int>& dims)
{
    return std::accumulate(dims.cbegin(), dims.cend(), 1, std::multiplies<int> {}) * formatNbByte[static_cast<int>(format)];
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

///////////////////////////////////////////////////////////////////////////////

void Stream::ping() const
{
    mSocket.write(Socket::Message::PING);
}

void Stream::close()
{
    mSocket.write(Socket::Message::CLOSE);
}

size_t Stream::getAcquisitionSize() const
{
    return mAcquisitionSize;
}

const std::string& Stream::getSensorName() const
{
    return mSensorName;
}

const std::vector<int>& Stream::getDims() const
{
    return mDims;
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

    try {
        mSocket.read(mFormat);
    } catch (Socket::exception& e) {
        std::cout << "[InputStream] catch exception : " << e.what() << std::endl;
        throw Stream::exception((std::string("sensor '") + sensorName + "' is not attached to server").c_str());
    }

    mSocket.read(mDims);

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
}

void InputStream::operator>>(Acquisition& acquisition) const
{
    assert(acquisition.acquisitionSize == mAcquisitionSize);

    mSocket.write(Socket::Message::SYNC);
    Socket::Message message;
    mSocket.read(message);

    switch (message) {
    case Socket::Message::DATA: {
        mSocket.read(acquisition.backendTimestamp);
        mSocket.read(acquisition.backendTimeOfArrival);
        mSocket.read(acquisition.data, mAcquisitionSize);
        std::cout << "[InputStream] read acq :  " << acquisition << std::endl;

    } break;

    case Socket::Message::CLOSE:
        std::cout << "[InputStream] request close" << std::endl;
        throw Socket::exception("server close connection");
        break;

    default:
        std::cout << "[InputStream] unknown message from server" << std::endl;
        exit(1);
    }
}

///////////////////////////////////////////////////////////////////////////////

OutputStream::OutputStream(const std::string& sensorName, Stream::Format format, const std::vector<int>& dims, const std::string& ipv4, int port)
    : Stream(sensorName, format, dims, ipv4, port)
{
    Client::Type clientType = Client::Type::STREAMER;
    mSocket.write(clientType);

    mSocket.write(mSensorName);
    mSocket.write(mFormat);
    mSocket.write(mDims);
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

void OutputStream::operator<<(const Acquisition& acquisition) const
{
    assert(acquisition.acquisitionSize == mAcquisitionSize);

    Socket::Message message;

    bool acquisitionSent = false;
    while (!acquisitionSent) {

        mSocket.read(message);

        switch (message) {
        case Socket::Message::PING:
            break;

        case Socket::Message::SYNC: {

            mSocket.write(Socket::Message::DATA);
            std::cout << "[OutputStream] send acq : " << acquisition << std::endl;

            mSocket.write(acquisition.backendTimestamp);
            mSocket.write(acquisition.backendTimeOfArrival);
            mSocket.write(acquisition.data, mAcquisitionSize);

            acquisitionSent = true;
        } break;

        default:
            std::cout << "[OutputStream] unknown message from server" << std::endl;
            exit(1);
        }
    }
}

void Stream::Acquisition::start()
{
    backendTimestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void Stream::Acquisition::end()
{
    backendTimeOfArrival = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
