

#include "Header.hpp"

//#include <algorithm>
#include <numeric>



 //const std::string& getSensorName() const;
 //Format getFormat() const;
 //const Dims& getDims() const;
 //const MetaData& getMetaData() const;

static constexpr int format2nByte[static_cast<int>( Header::Format::COUNT )] = {
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


size_t Header::computeAcquisitionSize( Format format, const Dims& dims ) {
    return std::accumulate( dims.cbegin(), dims.cend(), 1, std::multiplies<int> {} ) *
           format2nByte[static_cast<int>( format )];
}

const std::string& Header::getSensorName() const {
    return mSensorName;
}

Header::Format Header::getFormat() const {
    return mFormat;
}

const std::vector<int>& Header::getDims() const {
    return mDims;
}

const Header::MetaData& Header::getMetaData() const {
    return mMetaData;
}

size_t Header::getAcquisitionSize() const {
    return mAcquisitionSize;
}

 //static std::string dims2string( const Dims& dims );
 //static std::string metaData2string( const MetaData& metaData, bool expand = false );
 //static std::string any2string( const std::any& any );
 //SRC_API friend std::ostream& operator<<( std::ostream& os, const Format& format );

std::string Header::dims2string( const Dims& dims ) {
    std::string str = "";
    for ( unsigned int i = 0; i < dims.size(); ++i ) {
        str += std::to_string( dims[i] );
        if ( i != dims.size() - 1 ) { str += " x "; }
    }
    return str;
}

static constexpr char const* format2stringArray[static_cast<int>( Header::Format::COUNT )] = {
    "NONE",        "Z16",   "DISPARITY16", "XYZ32F",     "YUYV",          "RGB8",
    "BGR8",        "RGBA8", "BGRA8",       "Y8",         "Y16",           "RAW10",
    "RAW16",       "RAW8",  "UYVY",        "MOTION_RAW", "MOTION_XYZ32F", "GPIO_RAW",
    "DISPARITY32", "6DOF",  "Y10BPACK",    "DISTANCE",   "MJPEG",         "Y8I",
    "Y12I",        "INZI",  "INVI",        "W10",        "Z16H",          "FG",
    "Y411",
    // COUNT
};


std::string Header::format2string(const Format &format)
{
    return format2stringArray[(int)format];
}

std::string Header::metaData2string( const Header::MetaData& metaData, bool expand ) {
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

std::string Header::any2string( const std::any& any ) {
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
//                sprintf_s( buff, "%.1f", val->at( k ) );
                sprintf( buff, "%.1f", val->at( k ) );
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


std::ostream& operator<<( std::ostream& os, const Header::Format& format ) {
    os << Header::format2string(format) << " (byte:" << format2nByte[(int)format] << ")";
    return os;
}

