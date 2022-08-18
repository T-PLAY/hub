#include "Header.hpp"

//#include <algorithm>
#include <numeric>

// const std::string& getSensorName() const;
// Format getFormat() const;
// const Dims& getDims() const;
// const MetaData& getMetaData() const;
namespace hub {

static constexpr int format2nByte[static_cast<int>( Header::Format::COUNT )] = {
    0,       // NONE
    2,       // Z16
    2,       // DISPARITY16
    4,       // XYZ32F
    4,       // YUYV
    3,       // RGB8
    3,       // BGR8
    4,       // RGBA8
    4,       // BGRA8
    1,       // Y8
    2,       // Y16
    0,       // RAW10
    2,       // RAW16
    1,       // RAW8
    0,       // UYVY
    0,       // MOTION_RAW
    0,       // MOTION_XYZ32F
    0,       // GPIO_RAW
    0,       // DISPARITY32
    12 + 16, // 6DOF
    0,       // Y10BPACK
    4,       // DISTANCE
    0,       // MJPEG
    2,       // Y8I
    3,       // Y12I
    0,       // INZI
    1,       // INVI
    0,       // W10
    2,       // Z16H
    2,       // FG
    2,       // Y411
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

// static std::string dims2string( const Dims& dims );
// static std::string metaData2string( const MetaData& metaData, bool expand = false );
// static std::string any2string( const std::any& any );
// SRC_API friend std::ostream& operator<<( std::ostream& os, const Format& format );

std::string Header::dims2string( const Dims& dims ) {
    std::string str = "";
    for ( unsigned int i = 0; i < dims.size(); ++i ) {
        str += std::to_string( dims[i] );
        if ( i != dims.size() - 1 ) { str += " x "; }
    }
    return str;
}

static std::string format2stringArray[static_cast<int>( Header::Format::COUNT )] = {
    "NONE",        "Z16",   "DISPARITY16", "XYZ32F",     "YUYV",          "RGB8",
    "BGR8",        "RGBA8", "BGRA8",       "Y8",         "Y16",           "RAW10",
    "RAW16",       "RAW8",  "UYVY",        "MOTION_RAW", "MOTION_XYZ32F", "GPIO_RAW",
    "DISPARITY32", "6DOF",  "Y10BPACK",    "DISTANCE",   "MJPEG",         "Y8I",
    "Y12I",        "INZI",  "INVI",        "W10",        "Z16H",          "FG",
    "Y411",
};

std::string Header::format2string( const Format& format ) {
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
    else if ( hashCode == typeid( hub::io::Interface::Mat3 ).hash_code() ) {
        const hub::io::Interface::Mat3* val = std::any_cast<hub::io::Interface::Mat3>( &any );
        std::string str     = "";
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
    os << format2stringArray[(int)format] << " (byte:" << format2nByte[(int)format] << ")";
    return os;
}

} // namespace hub
