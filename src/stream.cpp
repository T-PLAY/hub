
#include "stream.h"

#include <cassert>
//#include <functional>
#include <numeric>
#include <socket.h>
//#include <math.h>
//#include <cmath>
//#include <algorithm>
//#include <FileIO.h>
#include <iomanip>
//#include <memory>
#include <algorithm>

std::ostream& operator<<(std::ostream& os, const Stream::Acquisition& acq)
{
    os << "start:" << acq.mBackendTimestamp / 1000 << ", end:" << acq.mBackendTimeOfArrival / 1000;
    os << ", data:[";
    for (auto i = 0; i < std::min((int)acq.mSize, 10); ++i) {
        os << std::setw(3) << (int)acq.mData[i] << " ";
    }
    os << "], ";
    os << 1'000'000.0 / (acq.mBackendTimeOfArrival - acq.mBackendTimestamp) << " fps";
    return os;
}

// Stream::Acquisition Stream::acquisition() const
//{
//}

// Stream::Stream(const std::string& sensorName, Format format, const std::vector<int>& dims, const std::string& ipv4, int port)
//{
//#ifdef DEBUG_MSG
//#endif
// }

// Stream::Stream(const std::string& sensorName, Format format, const std::vector<int>& dims, std::fstream& file)
//{
//#ifdef DEBUG_MSG
//#endif
// }

Stream::Stream(const std::string& sensorName, Format format, const std::vector<int>& dims, IOStream& ioStream)
    : mSensorName(sensorName)
    , mFormat(format)
    , mDims(dims)
    , mIOStream(ioStream)
    , mAcquisitionSize(computeAcquisitionSize(format, dims))
{
}

Stream::~Stream()
{
#ifdef DEBUG_MSG
    std::cout << "[Stream] ~Stream()" << std::endl;
#endif
    mIOStream.close();
    delete &mIOStream;
}

// Stream::Stream(ClientSocket&& clientSocket)
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

IOStream &Stream::getIOStream() const
{
    return mIOStream;
}

///////////////////////////////////////////////////////////////////////////////

// InputStream::InputStream(const std::string& sensorName, const std::string& syncSensorName, const std::string& ipv4, int port)
//{
//#ifdef DEBUG_MSG
//#endif

//}

// InputStream::InputStream(std::fstream& file)
//{
//#ifdef DEBUG_MSG
//#endif

//}

// InputStream::InputStream(IOStream&& ioStream)
//{

//}

// InputStream::InputStream(ClientSocket&& sock, const std::string& sensorName)
//{
//#ifdef DEBUG_MSG
//#endif

//}

// InputStream::InputStream(ClientSocket &&ioStream)
//{

//}

// InputStream::InputStream(FileIO &&ioStream)
//{

//}

InputStream::InputStream(const std::string & sensorName, const std::string& syncSensorName)
    // InputStream::InputStream(const std::string &sensorName, const std::string &syncSensorName)
    : Stream("", Format::NONE, {}, *std::move(new ClientSocket(sensorName, syncSensorName)))
{

    mIOStream.read(mSensorName);
    mIOStream.read(mFormat);
    mIOStream.read(mDims);
    //    std::cout << "[InputStream] before read metaData" << std::endl;
    mIOStream.read(mMetaData);
    //    std::cout << "[InputStream] after read metaData" << std::endl;

    mAcquisitionSize = computeAcquisitionSize(mFormat, mDims);
}

//InputStream::InputStream(RamIO &ramIO)
//    : Stream("", Format::NONE, {}, ramIO)
//{
//    mIOStream.read(mSensorName);
//    mIOStream.read(mFormat);
//    mIOStream.read(mDims);
//    mIOStream.read(mMetaData);

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
        int nReadAcqs = 0;
        while (true) {
            InputStream::operator>>(acq);
//            if (nReadAcqs < 10) {
//                std::cout << "read acquisition : " << acq << std::endl;
//            }
            acqs.emplace_back(std::move(acq));
            ++nReadAcqs;
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
//{

//}

// OutputStream::OutputStream(const std::string& sensorName, Format format, const std::vector<int>& dims, std::fstream& file)
//{

//}

OutputStream::OutputStream(const std::string& sensorName, Format format, const std::vector<int>& dims, ClientSocket&& ioStream, const MetaData& metaData)
    : Stream(sensorName, format, dims, *std::move(new ClientSocket(std::move(ioStream))))
{
    mIOStream.setupOutput(sensorName);

    mIOStream.write(mSensorName);
    mIOStream.write(mFormat);
    mIOStream.write(mDims);

    //    for (const auto& pair : metaData) {
    //        const auto& name = pair.first;
    //        const auto& val = pair.second;
    //        std::cout << "[OutputStream] metadata: " << val.type().name() << " " << name << " = '" << any::to_string(val) << "'" << std::endl;
    //    }
    mIOStream.write(metaData);
}

// OutputStream::OutputStream(const std::string &sensorName, Stream::Format format, const std::vector<int> &dims, FileIO &&ioStream)
//{

//}

// OutputStream::OutputStream(IOStream &&ioStream)
//{

//}

// OutputStream::OutputStream(IOStream && ioStream, const InputStream &inputStream)
//{

//}

// OutputStream::OutputStream(ClientSocket&& sock, const InputStream& inputStream)
//{

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

//////////////////////////////////////////////// Acquisition ///////////////////////////////////////////

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

//Stream::Acquisition::Acquisition(const Acquisition &acq)
//{

//}

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
