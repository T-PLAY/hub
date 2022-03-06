#include "stream.h"

#include <cassert>
//#include <functional>
#include <numeric>
#include <socket.h>
//#include <math.h>
//#include <cmath>
//#include <algorithm>

std::ostream& operator<<(std::ostream& os, const Stream::Acquisition& acq)
{
    os << "start:" << acq.mBackendTimestamp / 1000 << ", end:" << acq.mBackendTimeOfArrival / 1000;
    os << ", data:[";
    for (int i = 0; i < 10; ++i) {
        os << (int)acq.mData[i] << " ";
    }
    os << "]";
    return os;
}

// Stream::Acquisition Stream::acquisition() const
//{
//}

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
    if (acquisition.mData == nullptr) {
        acquisition.mData = new unsigned char[mAcquisitionSize];
        acquisition.mOwnData = true;
        acquisition.mSize = mAcquisitionSize;
    }

    assert(!acquisition.mOwnData || acquisition.mSize == mAcquisitionSize);
    assert(acquisition.mData != nullptr);

    mSocket.write(Socket::Message::SYNC);
    Socket::Message message;
    mSocket.read(message);

    switch (message) {
    case Socket::Message::DATA: {
        mSocket.read(acquisition.mBackendTimestamp);
        mSocket.read(acquisition.mBackendTimeOfArrival);
        mSocket.read(acquisition.mData, mAcquisitionSize);

#ifdef DEBUG_STREAM
        std::cout << "[InputStream] read acq :  " << acquisition << std::endl;
#endif

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
    assert(!acquisition.mOwnData || acquisition.mSize == mAcquisitionSize);
    assert(acquisition.mData != nullptr);

    Socket::Message message;

    bool acquisitionSent = false;
    while (!acquisitionSent) {

        mSocket.read(message);

        switch (message) {
        case Socket::Message::PING:
            break;

        case Socket::Message::SYNC: {

#ifdef DEBUG_STREAM
            std::cout << "[OutputStream] send acq : " << acquisition << std::endl;
#endif

            mSocket.write(Socket::Message::DATA);

            mSocket.write(acquisition.mBackendTimestamp);
            mSocket.write(acquisition.mBackendTimeOfArrival);
            mSocket.write(acquisition.mData, mAcquisitionSize);

            acquisitionSent = true;
        } break;

        default:
            std::cout << "[OutputStream] unknown message from server" << std::endl;
            exit(1);
        }
    }
}

// void Stream::Acquisition::start()
//{
//}

// void Stream::Acquisition::end()
//{
//}

// Stream::Acquisition::~Acquisition()
//{
// }

Stream::Acquisition::Acquisition(long long backendTimestamp, long long backendTimeOfArrival, unsigned char* data)
    : mBackendTimestamp(backendTimestamp)
    , mBackendTimeOfArrival(backendTimeOfArrival)
    , mData(data)
{
}

Stream::Acquisition::~Acquisition()
{
    if (mOwnData) {
        assert(mData != nullptr);
        delete[] mData;
    }
}
