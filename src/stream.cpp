
#include "stream.h"

#include <cassert>
//#include <functional>
#include <numeric>
#include <socket.h>
//#include <math.h>
//#include <cmath>
//#include <algorithm>
#include <FileIO.h>
#include <iomanip>
//#include <memory>
#include <algorithm>

std::ostream& operator<<(std::ostream& os, const Stream::Acquisition& acq)
{
    os << "start:" << acq.mBackendTimestamp / 1000 << ", end:" << acq.mBackendTimeOfArrival / 1000;
    os << ", data:[";
    for (auto i = 0; i < std::min((int)acq.mSize, 10); ++i) {
        os << std::setw(3) << (int)acq.mData[i] << " ";
        //        if (i % 4 == 3) {
        //            os << "\t";
        //        }
    }
    os << "], ";
    os << 1'000'000.0 / (acq.mBackendTimeOfArrival - acq.mBackendTimestamp) << " fps";
    return os;
}

// Stream::Acquisition Stream::acquisition() const
//{
//}

// Stream::Stream(const std::string& sensorName, Format format, const std::vector<int>& dims, const std::string& ipv4, int port)
//     : mSensorName(sensorName)
//     , mFormat(format)
//     , mDims(dims)
//     , mIOStream(new ClientSocket(ipv4, port))
//     , mAcquisitionSize(computeAcquisitionSize(format, dims))
//{
//#ifdef DEBUG_MSG
//     std::cout << "[Stream] Stream()" << std::endl;
//#endif
// }

// Stream::Stream(const std::string& sensorName, Format format, const std::vector<int>& dims, std::fstream& file)
//     : mSensorName(sensorName)
//     , mFormat(format)
//     , mDims(dims)
//     , mIOStream(new FileIO(file))
//     //    , mIOStream(std::make_unique<FileIO>(file))
//     , mAcquisitionSize(computeAcquisitionSize(format, dims))
//{
//#ifdef DEBUG_MSG
//     std::cout << "[Stream] Stream()" << std::endl;
//#endif
// }

Stream::Stream(const std::string& sensorName, Format format, const std::vector<int>& dims, const IOStream& ioStream)
    : mSensorName(sensorName)
    , mFormat(format)
    , mDims(dims)
    //        , mIOStream(std::make_unique<IOStream>(std::move(ioStream)))
    //    , mIOStream(ioStream)
    , mIOStream(ioStream)
    //    , mIOStream(std::move(ioStream))
    , mAcquisitionSize(computeAcquisitionSize(format, dims))
{
}

Stream::~Stream()
{
#ifdef DEBUG_MSG
    std::cout << "[Stream] ~Stream()" << std::endl;
#endif
    delete &mIOStream;
//    mIOStream = nullptr;
}

// Stream::Stream(ClientSocket&& clientSocket)
//     : mIOStream(new ClientSocket(std::move(clientSocket)))
//{
// }

size_t Stream::computeAcquisitionSize(Format format, const std::vector<int>& dims)
{
    return std::accumulate(dims.cbegin(), dims.cend(), 1, std::multiplies<int> {}) * format2nByte[static_cast<int>(format)];
}

// void Stream::waitClose()
//{

//}

///////////////////////////////////////////////////////////////////////////////

// void Stream::ping() const
//{
// }

// void Stream::close()
//{
// }

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

size_t Stream::getAcquisitionSize() const
{
    return mAcquisitionSize;
}

///////////////////////////////////////////////////////////////////////////////

// InputStream::InputStream(const std::string& sensorName, const std::string& syncSensorName, const std::string& ipv4, int port)
//     : Stream(sensorName, Format::NONE, {}, ipv4, port)
//{
//#ifdef DEBUG_MSG
//     std::cout << "[InputStream] InputStream(sensorName, ipv4, port)" << std::endl;
//#endif
//     ClientSocket::Type clientType = ClientSocket::Type::STREAM_VIEWER;
//     mIOStream.write(clientType);

//    mIOStream.write(sensorName);
//    Socket::Message mess;
//    mIOStream.read(mess);
//    if (mess == Socket::Message::NOT_FOUND) {
//        throw Stream::exception((std::string("sensor '") + sensorName + "' is not attached to server").c_str());
//    }
//    assert(mess == Socket::Message::OK);

//    mIOStream.write(syncSensorName);
//    mIOStream.read(mess);
//    if (mess == Socket::Message::NOT_FOUND) {
//        throw Stream::exception((std::string("sync sensor '") + syncSensorName + "' is not attached to server").c_str());
//    }
//    assert(mess == Socket::Message::OK);

//    //    try {
//    mIOStream.read(mFormat);
//    //    } catch (Socket::exception& e) {
//    //        std::cout << "[InputStream] catch exception : " << e.what() << std::endl;
//    //        throw Stream::exception((std::string("sensor '") + sensorName + "' is not attached to server").c_str());
//    //    }

//    mIOStream.read(mDims);

//    mAcquisitionSize = computeAcquisitionSize(mFormat, mDims);
//}

// InputStream::InputStream(std::fstream& file)
//     : Stream("", Format::NONE, {}, file)
//{
//#ifdef DEBUG_MSG
//     std::cout << "[InputStream] InputStream(sensorName, ipv4, port)" << std::endl;
//#endif

//    mIOStream.read(mSensorName);
//    //    mIOStream.read(mSensorName);
//    //    assert(sensorName == sensorNameInFile);

//    //    try {
//    mIOStream.read(mFormat);
//    //    } catch (Socket::exception& e) {
//    //        std::cout << "[InputStream] catch exception : " << e.what() << std::endl;
//    //        throw Stream::exception((std::string("sensor '") + sensorName + "' is not attached to server").c_str());
//    //    }

//    mIOStream.read(mDims);

//    mAcquisitionSize = computeAcquisitionSize(mFormat, mDims);
//}

InputStream::InputStream(const IOStream&& ioStream)
    //    : Stream("", Format::NONE, {}, std::move(ioStream))
    //    : Stream("", Format::NONE, {}, std::forward<IOStream&>(ioStream))
    : Stream("", Format::NONE, {}, std::move(ioStream))
{
    //    ioStream.setupInput();

    mIOStream.read(mSensorName);
    mIOStream.read(mFormat);
    mIOStream.read(mDims);
    //    mIOStream.read(mMetaData);

    mAcquisitionSize = computeAcquisitionSize(mFormat, mDims);
}

// InputStream::InputStream(ClientSocket&& sock, const std::string& sensorName)
//     : Stream(std::move(sock))
//{
//#ifdef DEBUG_MSG
//     std::cout << "[InputStream] InputStream(ClientSocket && sock)" << std::endl;
//#endif
//     mSensorName = sensorName;
//     //    mIOStream.read(mSensorName);
//     mIOStream.read(mFormat);
//     mIOStream.read(mDims);

//    mAcquisitionSize = computeAcquisitionSize(mFormat, mDims);
//}

InputStream::~InputStream()
{
#ifdef DEBUG_MSG
    std::cout << "[InputStream] ~InputStream()" << std::endl;
#endif
}

Stream::Acquisition& InputStream::operator>>(Acquisition& acquisition) const
{
    if (acquisition.mOwnData && acquisition.mSize != mAcquisitionSize) {
        delete[] acquisition.mData;
        acquisition.mOwnData = false;
        acquisition.mData = nullptr;
    }
    if (acquisition.mData == nullptr) {
        acquisition.mData = new unsigned char[mAcquisitionSize];
        acquisition.mOwnData = true;
        acquisition.mSize = mAcquisitionSize;
    }

    assert(!acquisition.mOwnData || acquisition.mSize == mAcquisitionSize);
    assert(acquisition.mData != nullptr);

    mIOStream.read(acquisition.mBackendTimestamp);
    mIOStream.read(acquisition.mBackendTimeOfArrival);
    mIOStream.read(acquisition.mData, mAcquisitionSize);

    assert(acquisition.mBackendTimestamp <= acquisition.mBackendTimeOfArrival);

#ifdef DEBUG_STREAM
    std::cout << "[InputStream] read acq :  " << acquisition << std::endl;
#endif

    return acquisition;
}

std::vector<Stream::Acquisition> InputStream::getAllAcquisition()
{
    std::vector<Stream::Acquisition> acqs;

    try {
        Stream::Acquisition acq;
        while (true) {
            InputStream::operator>>(acq);
            acqs.emplace_back(std::move(acq));
        }
    } catch (Stream::exception& e) {
        std::cout << "[stream] catch stream exception : " << e.what() << std::endl;
        throw;

    } catch (std::exception& e) {
        std::cout << "[stream] catch exception : " << e.what() << std::endl;
    }

    return acqs;
}

const Stream::MetaData& InputStream::getMetaData() const
{
    return mMetaData;
}

void InputStream::operator>>(const OutputStream& outputStream) const
{
    Stream::Acquisition acq;
    InputStream::operator>>(acq);
    outputStream << std::move(acq);
}

///////////////////////////////////////////////////////////////////////////////

// OutputStream::OutputStream(const std::string& sensorName, Stream::Format format, const std::vector<int>& dims, const MetaData& metaData, const std::string& ipv4, int port)
//     : Stream(sensorName, format, dims, ipv4, port)
//{
//     ClientSocket::Type clientType = ClientSocket::Type::STREAMER;
//     mIOStream.write(clientType);

//    mIOStream.write(mSensorName);
//    Socket::Message mess;
//    mIOStream.read(mess);
//    if (mess == Socket::Message::FOUND) {
//        throw Stream::exception((std::string("sensor '") + sensorName + "' is already attached to server").c_str());
//    }
//    assert(mess == Socket::Message::NOT_FOUND);

//    mIOStream.write(mFormat);
//    mIOStream.write(mDims);
//}

// OutputStream::OutputStream(const std::string& sensorName, Format format, const std::vector<int>& dims, std::fstream& file)
//     : Stream(sensorName, format, dims, file)
//{

//    mIOStream.write(mSensorName);
//    mIOStream.write(mFormat);
//    mIOStream.write(mDims);
//}

//OutputStream::OutputStream(const std::string& sensorName, Format format, const std::vector<int>& dims, const IOStream &ioStream, const MetaData& metaData)
//    //    : Stream(sensorName, format, dims, dynamic_cast<IOStream&>(ioStream))
//    : Stream(sensorName, format, dims, std::move(ioStream))
////    : Stream(sensorName, format, dims, std::move(ioStream))
////    : Stream(sensorName, format, dims, std::forward<IOStream&>(ioStream))
//{
//    //    mIOStream.setupMode(IOStream::Mode::OUTPUT);
//    //    mIOStream.init(sensorName);
//    mIOStream.setupOutput(sensorName);

//    mIOStream.write(mSensorName);
//    mIOStream.write(mFormat);
//    mIOStream.write(mDims);
//    //    mIOStream.write(metaData);
//}

// OutputStream::OutputStream(IOStream &&ioStream)
//     : Stream("", Format::NONE, {}, std::move(ioStream))
//{

//}

// OutputStream::OutputStream(IOStream && ioStream, const InputStream &inputStream)
////    : Stream("", Format::BGR8, {}, std::move(ioStream))
//    : Stream("", Format::NONE, {}, std::move(ioStream))
////    : Stream("",
//{

//}

// OutputStream::OutputStream(ClientSocket&& sock, const InputStream& inputStream)
//     : Stream(std::move(sock))
//{
//     mSensorName = inputStream.getSensorName();
//     mFormat = inputStream.getFormat();
//     mDims = inputStream.getDims();

//    mAcquisitionSize = computeAcquisitionSize(mFormat, mDims);

//    mIOStream.write(mSensorName);
//    mIOStream.write(mFormat);
//    mIOStream.write(mDims);
//}

void OutputStream::operator<<(const Acquisition& acquisition) const
{
    assert(!acquisition.mOwnData || acquisition.mSize == mAcquisitionSize);
    assert(acquisition.mData != nullptr);

    assert(acquisition.mBackendTimestamp <= acquisition.mBackendTimeOfArrival);

    //#ifdef DEBUG_STREAM
    //#endif

    mIOStream.write(acquisition.mBackendTimestamp);
    mIOStream.write(acquisition.mBackendTimeOfArrival);
    mIOStream.write(acquisition.mData, mAcquisitionSize);
}

void OutputStream::operator<<(const InputStream& inputStream) const
{
    Stream::Acquisition acq;
    inputStream >> acq;
    OutputStream::operator<<(std::move(acq));
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

// Stream::Acquisition::Acquisition(Acquisition &&acq)
//{

//}

// Stream::Acquisition &Stream::Acquisition::operator=(Acquisition &&acq)
//{

//}

// Stream::Acquisition::Acquisition(const Acquisition& acq)
//{
// }

Stream::Acquisition::~Acquisition()
{
    if (mOwnData) {
        assert(mData != nullptr);
        delete[] mData;
        mOwnData = false;
        mData = nullptr;
    }
}

Stream::Acquisition::Acquisition(Acquisition&& acq)
    : mBackendTimestamp(acq.mBackendTimestamp)
    , mBackendTimeOfArrival(acq.mBackendTimeOfArrival)
    , mData(acq.mData)
    , mSize(acq.mSize)
    , mOwnData(acq.mOwnData)
{
    acq.mOwnData = false;
    acq.mData = nullptr;
}

Stream::Acquisition& Stream::Acquisition::operator=(Acquisition&& acq)
{
    mBackendTimestamp = acq.mBackendTimestamp;
    mBackendTimeOfArrival = acq.mBackendTimeOfArrival;
    mData = acq.mData;
    mSize = acq.mSize;
    mOwnData = acq.mOwnData;

    acq.mOwnData = false;
    acq.mData = nullptr;

    return *this;
}

Stream::Acquisition Stream::Acquisition::clone() const
{
    assert(mData != nullptr);

    Stream::Acquisition acq(mBackendTimestamp, mBackendTimeOfArrival, nullptr);
    acq.mSize = mSize;
    acq.mData = new unsigned char[mSize];
    acq.mOwnData = true;
    memcpy(acq.mData, mData, mSize);
    return acq;
}
