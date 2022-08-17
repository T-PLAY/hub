
#include "stream.h"

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <numeric>
#include <socket.h>

//#define DEBUG_STREAM

std::ostream& operator<<( std::ostream& os, const Stream::Acquisition& acq ) {
    os << "start:" << acq.mBackendTimestamp / 1000 << ", end:" << acq.mBackendTimeOfArrival / 1000;
    os << ", data:[";
    for ( auto i = 0; i < std::min( (int)acq.mSize, 10 ); ++i ) {
        os << std::setw( 3 ) << (int)acq.mData[i] << " ";
    }
    os << "], ";
    os << 1'000'000.0 / ( acq.mBackendTimeOfArrival - acq.mBackendTimestamp ) << " fps";
    return os;
}

Stream::Stream( const std::string& sensorName,
                Format format,
                const std::vector<int>& dims,
                IOStream& ioStream ) :
    mSensorName( sensorName ),
    mFormat( format ),
    mDims( dims ),
    mIOStream( ioStream ),
    mAcquisitionSize( computeAcquisitionSize( format, dims ) ) {}

Stream::~Stream() {
#ifdef DEBUG_MSG
    std::cout << "[Stream] ~Stream()" << std::endl;
#endif
    mIOStream.close();
    delete &mIOStream;
}

static constexpr char const* format2string[static_cast<int>( Stream::Format::COUNT )] = {
    "NONE",        "Z16",   "DISPARITY16", "XYZ32F",     "YUYV",          "RGB8",
    "BGR8",        "RGBA8", "BGRA8",       "Y8",         "Y16",           "RAW10",
    "RAW16",       "RAW8",  "UYVY",        "MOTION_RAW", "MOTION_XYZ32F", "GPIO_RAW",
    "DISPARITY32", "6DOF",  "Y10BPACK",    "DISTANCE",   "MJPEG",         "Y8I",
    "Y12I",        "INZI",  "INVI",        "W10",        "Z16H",          "FG",
    "Y411",
    // COUNT
};

static constexpr int format2nByte[static_cast<int>( Stream::Format::COUNT )] = {
    0, // NONE
    2, // Z16             , /**< 16-bit linear depth values. The depth is meters is equal to depth
       // scale * pixel value. */
    2, // DISPARITY16     , /**< 16-bit float-point disparity values. Depth->Disparity conversion :
       // Disparity = Baseline*FocalLength/Depth. */
    4, // XYZ32F          , /**< 32-bit floating point 3D coordinates. */
    4, // YUYV            , /**< 32-bit y0, u, y1, v data for every two pixels. Similar to YUV422
       // but packed in a different order - https://en.wikipedia.org/wiki/YUV */
    3, // RGB8            , /**< 8-bit red, green and blue channels */
    3, // BGR8            , /**< 8-bit blue, green, and red channels -- suitable for OpenCV */
    4, // RGBA8           , /**< 8-bit red, green and blue channels + constant alpha channel equal
       // to FF */
    4, // BGRA8           , /**< 8-bit blue, green, and red channels + constant alpha channel equal
       // to FF */
    1, // Y8              , /**< 8-bit per-pixel grayscale image */
    2, // Y16             , /**< 16-bit per-pixel grayscale image */
    0, // RAW10           , /**< Four 10 bits per pixel luminance values packed into a 5-byte
       // macropixel */
    2, // RAW16           , /**< 16-bit raw image */
    1, // RAW8            , /**< 8-bit raw image */
    0, // UYVY            , /**< Similar to the standard YUYV pixel format, but packed in a
       // different order */
    0, // MOTION_RAW      , /**< Raw data from the motion sensor */
    0, // MOTION_XYZ32F   , /**< Motion data packed as 3 32-bit float values, for X, Y, and Z axis
       // */
    0, // GPIO_RAW        , /**< Raw data from the external sensors hooked to one of the GPIO's */
    0, // DISPARITY32     , /**< 32-bit float-point disparity values. Depth->Disparity conversion :
       // Disparity = Baseline*FocalLength/Depth */
    12 + 16, // 6DOF           , /**< Pose data packed as floats array, containing translation
             // vector, rotation quaternion and prediction velocities and accelerations vectors */
    0, // Y10BPACK        , /**< 16-bit per-pixel grayscale image unpacked from 10 bits per pixel
       // packed ([8:8:8:8:2222]) grey-scale image. The data is unpacked to LSB and padded with 6
       // zero bits */
    4, // DISTANCE        , /**< 32-bit float-point depth distance value.  */
    0, // MJPEG           , /**< Bitstream encoding for video in which an image of each frame is
       // encoded as JPEG-DIB   */
    2, // Y8I             , /**< 8-bit per pixel interleaved. 8-bit left, 8-bit right.  */
    3, // Y12I            , /**< 12-bit per pixel interleaved. 12-bit left, 12-bit right. Each pixel
       // is stored in a 24-bit word in little-endian order. */
    0, // INZI            , /**< multi-planar Depth 16bit + IR 10bit.  */
    1, // INVI            , /**< 8-bit IR stream.  */
    0, // W10             , /**< Grey-scale image as a bit-packed array. 4 pixel data stream taking
       // 5 bytes */
    2, // Z16H            , /**< Variable-length Huffman-compressed 16-bit depth values. */
    2, // FG              , /**< 16-bit per-pixel frame grabber format. */
    2, // Y411            , /**< 12-bit per-pixel. */
    // COUNT             	/**< Number of enumeration values. Not a valid input: intended to be used
    // in for-loops. */
};

std::ostream& operator<<( std::ostream& os, const Stream::Format& format ) {
    os << format2string[(int)format] << " (byte:" << format2nByte[(int)format] << ")";
    return os;
}

std::string Stream::dims2string( const std::vector<int>& dims ) {
    std::string str = "";
    for ( unsigned int i = 0; i < dims.size(); ++i ) {
        str += std::to_string( dims[i] );
        if ( i != dims.size() - 1 ) { str += " x "; }
    }
    return str;
}

std::string Stream::metaData2string( const Stream::MetaData& metaData, bool expand ) {
    std::string str = "";
    if ( expand ) {
        bool first = true;
        for ( const auto& pair : metaData ) {
            if ( first )
                first = false;
            else
                str += "\n";

            const auto& name = pair.first;
            const auto& val  = pair.second;
            str += std::string( val.type().name() ) + " " + name + " = '" + any2string( val ) + "'";
        }
    }
    else {
        str += "[";
        for ( const auto& pair : metaData ) {
            const auto& name = pair.first;
            const auto& val  = pair.second;
            str +=
                std::string( val.type().name() ) + " " + name + " = '" + any2string( val ) + "', ";
        }
        str += "]";
    }
    return str;
}

std::string Stream::any2string( const std::any& any ) {
    assert( any.has_value() );
    const auto& hashCode = any.type().hash_code();

    if ( hashCode == typeid( double ).hash_code() ) {
        const double* val = std::any_cast<double>( &any );
        return std::to_string( *val );
    }
    else if ( hashCode == typeid( std::string ).hash_code() ) {
        const std::string* val = std::any_cast<std::string>( &any );
        return std::string( *val );
    }
    else if ( hashCode == typeid( const char* ).hash_code() ) {
        const char* val = *std::any_cast<const char*>( &any );
        return std::string( val );
    }
    else if ( hashCode == typeid( int ).hash_code() ) {
        const int* val = std::any_cast<int>( &any );
        return std::to_string( *val );
    }
    else if ( hashCode == typeid( std::vector<float> ).hash_code() ) {
        const std::vector<float>* val = std::any_cast<std::vector<float>>( &any );
        std::string str               = "";
        const int n                   = 3;
        for ( int i = 0; i < n; ++i ) {
            for ( int j = 0; j < n; ++j ) {
                char buff[32];
                const int k = i * n + j;
                // sprintf(buff, "%.1f", val->at(k));
                sprintf_s( buff, "%.1f", val->at( k ) );
                str += buff;
                //            str += std::to_string(val->at(i));
                if ( j != 2 ) str += " ";
            }
            if ( i != 2 ) str += "  ";
        }
        return str;
    }
    else if ( hashCode == typeid( IOStream::Mat3 ).hash_code() ) {
        const IOStream::Mat3* val = std::any_cast<IOStream::Mat3>( &any );
        std::string str           = "";
        for ( int i = 0; i < 3; ++i ) {
            for ( int j = 0; j < 3; ++j ) {
                str += std::to_string( val->data[i * 3 + j] ) + " ";
            }
            str += "\n";
        }
        return str;
    }
    else {
        auto name     = any.type().name();
        auto raw_name = any.type().name();
        assert( false );
    }
    return "";
}

size_t Stream::computeAcquisitionSize( Format format, const std::vector<int>& dims ) {
    return std::accumulate( dims.cbegin(), dims.cend(), 1, std::multiplies<int> {} ) *
           format2nByte[static_cast<int>( format )];
}

std::ostream& operator<<( std::ostream& os, const Stream& stream ) {
    //        os << stream.mSensorName << stream.mFormat << stream.mDims << stream.mAcquisitionSize;
    os << stream.mSensorName;
    return os;
}

///////////////////////////////////////////////////////////////////////////////

const std::string& Stream::getSensorName() const {
    return mSensorName;
}

const std::vector<int>& Stream::getDims() const {
    return mDims;
}

Stream::Format Stream::getFormat() const {
    return mFormat;
}

size_t Stream::getAcquisitionSize() const {
    return mAcquisitionSize;
}

IOStream& Stream::getIOStream() const {
    return mIOStream;
}

///////////////////////////////////////////////////////////////////////////////

InputStream::InputStream( const std::string& sensorName, const std::string& syncSensorName ) :
    Stream( "", Format::NONE, {}, *std::move( new ClientSocket( sensorName, syncSensorName ) ) ) {

    mIOStream.read( mSensorName );
    mIOStream.read( mFormat );
    mIOStream.read( mDims );
    mIOStream.read( mMetaData );

    mAcquisitionSize = computeAcquisitionSize( mFormat, mDims );
}

InputStream::~InputStream() {
#ifdef DEBUG_MSG
    std::cout << "[InputStream] ~InputStream()" << std::endl;
#endif
}

Stream::Acquisition InputStream::getAcquisition() const {
    long long start, end;
    unsigned char* data = new unsigned char[mAcquisitionSize];

    mIOStream.read( start );
    mIOStream.read( end );
    mIOStream.read( data, mAcquisitionSize );

#ifdef DEBUG_STREAM
    std::cout << "[InputStream] read acq :  " << acquisition << std::endl;
#endif

    return Stream::Acquisition( start, end, data, mAcquisitionSize );
    delete[] data;
}

std::vector<Stream::Acquisition> InputStream::getAllAcquisition() {
    std::vector<Stream::Acquisition> acqs;

    try {
        int nReadAcqs = 0;
        while ( true ) {
            acqs.emplace_back( getAcquisition() );
            ++nReadAcqs;
        }
    }
    catch ( Stream::exception& e ) {
        std::cout << "[stream] catch stream exception : " << e.what() << std::endl;
        throw;
    }
    catch ( std::exception& e ) {
        std::cout << "[stream] catch exception : " << e.what() << std::endl;
    }

    return acqs;
}

const Stream::MetaData& InputStream::getMetaData() const {
    return mMetaData;
}

std::ostream& operator<<( std::ostream& os, const InputStream& inputStream ) {
    os << "metadata:" << InputStream::metaData2string( inputStream.mMetaData );
    return os;
}

///////////////////////////////////////////////////////////////////////////////

OutputStream::OutputStream( const std::string& sensorName,
                            Format format,
                            const std::vector<int>& dims,
                            ClientSocket&& ioStream,
                            const MetaData& metaData ) :
    Stream( sensorName, format, dims, *std::move( new ClientSocket( std::move( ioStream ) ) ) ) {
    mIOStream.setupOutput( sensorName );

    mIOStream.write( mSensorName );
    mIOStream.write( mFormat );
    mIOStream.write( mDims );

    mIOStream.write( metaData );
}

void OutputStream::operator<<( const Acquisition& acquisition ) const {
    assert( acquisition.mData != nullptr );
    assert( acquisition.mSize == mAcquisitionSize );

    assert( acquisition.mBackendTimestamp <= acquisition.mBackendTimeOfArrival );

    mIOStream.write( acquisition.mBackendTimestamp );
    mIOStream.write( acquisition.mBackendTimeOfArrival );
    mIOStream.write( acquisition.mData, mAcquisitionSize );
}

//////////////////////////////////////////////// Acquisition
//////////////////////////////////////////////

Stream::Acquisition::Acquisition( long long backendTimestamp,
                                  long long backendTimeOfArrival,
                                  const unsigned char* const data,
                                  size_t size ) :
    mBackendTimestamp( backendTimestamp ),
    mBackendTimeOfArrival( backendTimeOfArrival ),
    mData( new unsigned char[size] ),
    mSize( size ) {

    assert( data != nullptr );
    memcpy( (unsigned char*)mData, data, size );
    assert( size > 0 );
    assert( backendTimestamp <= backendTimeOfArrival );
}

Stream::Acquisition::~Acquisition() {
    assert( mData != nullptr );

    if ( !mIsMoved ) { delete[] mData; }
}

Stream::Acquisition::Acquisition( Acquisition&& acq ) noexcept :
    mBackendTimestamp( acq.mBackendTimestamp ),
    mBackendTimeOfArrival( acq.mBackendTimeOfArrival ),
    mData( acq.mData ),
    mSize( acq.mSize ) {
    acq.mIsMoved = true;
}

Stream::Acquisition Stream::Acquisition::clone() const {
    assert( mData != nullptr );

    return Stream::Acquisition( mBackendTimestamp, mBackendTimeOfArrival, mData, mSize );
}
