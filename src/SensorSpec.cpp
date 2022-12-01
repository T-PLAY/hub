#include "SensorSpec.hpp"

#include <cassert>
//#include <numeric>

#include "IO/Interface.hpp"

namespace hub {

#if CPLUSPLUS_VERSION != 20
SensorSpec::SensorSpec( const SensorNameType& sensorName,
                         const Resolutions& resolutions,
                         const MetaData& metaData ) :
     m_sensorName( sensorName ),
     m_resolutions( resolutions ),
     m_metaData( metaData ),
     m_acquisitionSize( computeAcquisitionSize( resolutions ) ) {}
#endif


// constexpr SensorSpec::SensorSpec(
////        const std::string_view &sensorName, const Resolutions &resolutions,
////        const MetaData &metaData
//        )
//{

//}

//constexpr SensorSpec::SensorSpec( const std::string_view& sensorName,
//                        const Resolutions& resolutions,
//                        const MetaData& metaData ) :
//    m_sensorName( sensorName ),
//    m_resolutions( resolutions ),
//    m_metaData( metaData ),
//    m_acquisitionSize( computeAcquisitionSize( resolutions ) ) {}

SensorSpec SensorSpec::operator+( const SensorSpec& sensorSpec ) const {
    //    SensorSpec ret;
    std::string sensorName;
    hub::SensorSpec::Resolutions resolutions;
    hub::SensorSpec::MetaData metaData;
    // todo
    //    sensorName  = m_sensorName + " + " + sensorSpec.m_sensorName;
    resolutions = m_resolutions;
    resolutions.insert(
        resolutions.end(), sensorSpec.m_resolutions.begin(), sensorSpec.m_resolutions.end() );
    metaData = m_metaData;

    metaData.insert( sensorSpec.m_metaData.begin(), sensorSpec.m_metaData.end() );
    metaData.erase( "parent" );

    //    ret.m_acquisitionSize = m_acquisitionSize + sensorSpec.m_acquisitionSize;
    //    assert( ret.m_acquisitionSize == computeAcquisitionSize( ret.m_resolutions ) );
    //    return ret;
    return SensorSpec( std::move( sensorName ), std::move( resolutions ), std::move( metaData ) );

    //    ret.m_sensorName  = m_sensorName + " + " + sensorSpec.m_sensorName;
    //    ret.m_resolutions = m_resolutions;
    //    ret.m_resolutions.insert(
    //        ret.m_resolutions.end(), sensorSpec.m_resolutions.begin(),
    //        sensorSpec.m_resolutions.end() );
    //    ret.m_metaData = m_metaData;
    //    ret.m_metaData.insert( sensorSpec.m_metaData.begin(), sensorSpec.m_metaData.end() );
    //    ret.m_metaData.erase( "parent" );

    //    ret.m_acquisitionSize = m_acquisitionSize + sensorSpec.m_acquisitionSize;
    //    assert( ret.m_acquisitionSize == computeAcquisitionSize( ret.m_resolutions ) );
    //    return ret;
}

SensorSpec& SensorSpec::operator+=( const SensorSpec& sensorSpec ) {
    if ( m_sensorName == "" ) { *this = sensorSpec; }
    else { *this = *this + sensorSpec; }
    return *this;
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

// constexpr int SensorSpec::format2nByte(const Format &format)
//{
//     return s_format2nByte[static_cast<int>(format)];
// }

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

std::ostream& operator<<( std::ostream& os, const SensorSpec::Format& format ) {
    os << format2stringArray[(int)format] << " (byte:" << s_format2nByte[(int)format] << ")";
    return os;
}

std::ostream& operator<<( std::ostream& os, const SensorSpec& sensorSpec ) {
    os << sensorSpec.m_sensorName << ", "
       << SensorSpec::resolutions2string( sensorSpec.m_resolutions ) << ", "
       << SensorSpec::metaData2string( sensorSpec.m_metaData ) << ", "
       << sensorSpec.m_acquisitionSize;
    return os;
}

// std::string SensorSpec::getSensorName() const
//{
//     return m_sensorName;
// }

// hub::SensorSpec::MetaData SensorSpec::getMetaData() const
//{
//     return m_metaData;
// }

// hub::SensorSpec::Resolutions SensorSpec::getResolutions() const
//{
//     return m_resolutions;
// }

} // namespace hub
