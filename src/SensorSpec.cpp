#include "SensorSpec.hpp"

#include <cassert>
#include <numeric>

#include "IO/Interface.hpp"

namespace hub {

static constexpr int format2nByte[static_cast<int>( SensorSpec::Format::COUNT )] = {
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
    12 + 16, // DOF6
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
    64,      // MAT4
};

static constexpr bool format2interpolable[static_cast<int>( SensorSpec::Format::COUNT )] = {
    false, // NONE
    false, // Z16
    false, // DISPARITY16
    false, // XYZ32F
    false, // YUYV
    false, // RGB8
    false, // BGR8
    false, // RGBA8
    false, // BGRA8
    false, // Y8
    false, // Y16
    false, // RAW10
    false, // RAW16
    false, // RAW8
    false, // UYVY
    false, // MOTION_RAW
    false, // MOTION_XYZ32F
    false, // GPIO_RAW
    false, // DISPARITY32
    true,  // DOF6
    false, // Y10BPACK
    false, // DISTANCE
    false, // MJPEG
    false, // Y8I
    false, // Y12I
    false, // INZI
    false, // INVI
    false, // W10
    false, // Z16H
    false, // FG
    false, // Y411
    false, // MAT4
};

size_t SensorSpec::computeAcquisitionSize( const Resolutions& resolutions ) {
    size_t size = 0;
    for ( const auto& resolution : resolutions ) {
        size += computeAcquisitionSize( resolution );
    }
    return size;
}

size_t SensorSpec::computeAcquisitionSize( const Resolution& resolution ) {
    const auto& dims   = resolution.first;
    const auto& format = resolution.second;
    return computeAcquisitionSize( format, dims );
}

SensorSpec SensorSpec::operator+( const SensorSpec& sensorSpec ) const {
    SensorSpec ret;
    ret.m_sensorName  = m_sensorName + " + " + sensorSpec.m_sensorName;
    ret.m_resolutions = m_resolutions;
    ret.m_resolutions.insert(
        ret.m_resolutions.end(), sensorSpec.m_resolutions.begin(), sensorSpec.m_resolutions.end() );
    ret.m_metaData = m_metaData;
    ret.m_metaData.insert( sensorSpec.m_metaData.begin(), sensorSpec.m_metaData.end() );
    ret.m_metaData.erase( "parent" );

    ret.m_acquisitionSize = m_acquisitionSize + sensorSpec.m_acquisitionSize;
    assert( ret.m_acquisitionSize == computeAcquisitionSize( ret.m_resolutions ) );
    return ret;
}

SensorSpec& SensorSpec::operator+=( const SensorSpec& sensorSpec ) {
    if ( m_sensorName == "" ) { *this = sensorSpec; }
    else { *this = *this + sensorSpec; }
    return *this;
}

size_t SensorSpec::computeAcquisitionSize( Format format, const Dims& dims ) {
    return std::accumulate( dims.cbegin(), dims.cend(), 1, std::multiplies<int> {} ) *
           format2nByte[static_cast<int>( format )];
}

std::string SensorSpec::dims2string( const Dims& dims ) {
    std::string str = "";
    for ( unsigned int i = 0; i < dims.size(); ++i ) {
        str += std::to_string( dims[i] );
        if ( i != dims.size() - 1 ) { str += " x "; }
    }
    return str;
}

static std::string format2stringArray[static_cast<int>( SensorSpec::Format::COUNT )] = {
    "NONE",        "Z16",   "DISPARITY16", "XYZ32F",     "YUYV",          "RGB8",
    "BGR8",        "RGBA8", "BGRA8",       "Y8",         "Y16",           "RAW10",
    "RAW16",       "RAW8",  "UYVY",        "MOTION_RAW", "MOTION_XYZ32F", "GPIO_RAW",
    "DISPARITY32", "6DOF",  "Y10BPACK",    "DISTANCE",   "MJPEG",         "Y8I",
    "Y12I",        "INZI",  "INVI",        "W10",        "Z16H",          "FG",
    "Y411",        "MAT4",
};

std::string SensorSpec::format2string( const Format& format ) {
    return format2stringArray[(int)format];
}

std::string SensorSpec::resolutions2string( const Resolutions& resolutions ) {
    const int size  = static_cast<int>( resolutions.size() );
    std::string str = "";
    if ( size > 1 ) str += "[";
    for ( int i = 0; i < size; ++i ) {
        const auto& dims   = resolutions[i].first;
        const auto& format = resolutions[i].second;
        if ( size > 1 ) str += "{";
        str += dims2string( dims ) + " : " + format2string( format );
        if ( size > 1 ) str += "}";
        if ( i != size - 1 ) str += ", ";
    }
    if ( size > 1 ) str += "]";
    return str;
}

std::string SensorSpec::metaData2string( const SensorSpec::MetaData& metaData, bool expand ) {
    std::string str = "";
    if ( expand ) {
        bool first = true;
        for ( const auto& pair : metaData ) {
            if ( first )
                first = false;
            else
                str += "\n";
            str += metaData2string( pair );
        }
    }
    else {
        str += "[";
        for ( const auto& pair : metaData ) {
            str += metaData2string( pair ) + ", ";
        }
        str += "]";
    }
    return str;
}

std::string SensorSpec::metaData2string( const std::pair<std::string, std::any>& metaData ) {
    const auto& name = metaData.first;
    const auto& val  = metaData.second;
    //#ifdef WIN32
    //#else
    std::string str = hub::io::Interface::anyType2string( val ) + " " + name + " = '" +
                      hub::io::Interface::anyValue2string( val ) + "'";
    //#endif
    return str;
}

bool SensorSpec::interpolable( const Format& format ) {
    return format2interpolable[(int)format];
}

std::ostream& operator<<( std::ostream& os, const SensorSpec::Format& format ) {
    os << format2stringArray[(int)format] << " (byte:" << format2nByte[(int)format] << ")";
    return os;
}

std::ostream& operator<<( std::ostream& os, const SensorSpec& sensorSpec ) {
    os << sensorSpec.m_sensorName << ", "
       << SensorSpec::resolutions2string( sensorSpec.m_resolutions ) << ", "
       << SensorSpec::metaData2string( sensorSpec.m_metaData ) << ", "
       << sensorSpec.m_acquisitionSize;
    return os;
}

} // namespace hub
