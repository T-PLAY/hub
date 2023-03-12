#include "SensorSpec.hpp"

#include <cassert>
// #include <numeric>

//#include "IO/Interface.hpp"
#include "Any.hpp"

namespace hub {

#if CPLUSPLUS_VERSION != 20
SensorSpec::SensorSpec( const SensorNameType& sensorName,
                        const Resolutions& resolutions,
                        const MetaData& metaData ) :
    m_sensorName( sensorName ),
    m_resolutions( resolutions ),
    m_metaData( metaData ),
    m_acquisitionSize( computeAcquisitionSize( resolutions ) ) {

//    assert(! m_resolutions.empty());

}
#endif

// constexpr SensorSpec::SensorSpec(
////        const std::string_view &sensorName, const Resolutions &resolutions,
////        const MetaData &metaData
//        )
//{

//}

// constexpr SensorSpec::SensorSpec( const std::string_view& sensorName,
//                         const Resolutions& resolutions,
//                         const MetaData& metaData ) :
//     m_sensorName( sensorName ),
//     m_resolutions( resolutions ),
//     m_metaData( metaData ),
//     m_acquisitionSize( computeAcquisitionSize( resolutions ) ) {}

SensorSpec SensorSpec::operator+( const SensorSpec& sensorSpec ) const {
    //    SensorSpec ret;
    std::string sensorName;
    Resolutions resolutions;
    SensorSpec::MetaData metaData;
    // todo
    sensorName  = m_sensorName + " + " + sensorSpec.m_sensorName;
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

bool SensorSpec::operator==( const SensorSpec& sensorSpec ) const {

    return  m_sensorName == sensorSpec.m_sensorName && m_resolutions == sensorSpec.m_resolutions &&
           m_acquisitionSize == sensorSpec.m_acquisitionSize && m_metaData.size() == sensorSpec.m_metaData.size();
    // todo any compare
//    if ( m_sensorName == sensorSpec.m_sensorName && m_resolutions == sensorSpec.m_resolutions &&
//         m_acquisitionSize == sensorSpec.m_acquisitionSize && m_metaData.size() == sensorSpec.m_metaData.size()) {

//        for (const auto & pair : m_metaData) {
//            const auto & name = pair.first;
//            const std::any & value = pair.second;

//            if (sensorSpec.m_metaData.find(name) != sensorSpec.m_metaData.end()) {
//                const std::any & value2 = sensorSpec.m_metaData.at(name);
//                if (value. value2) {

//                }

//            }
//            else {
//                return false;
//            }
//        }
//    }
//    else {
//        return false;
//    }
//    return true;
}

std::string SensorSpec::to_string() const {
    return m_sensorName + ", " + hub::to_string( m_resolutions ) + ", " +
           SensorSpec::to_string( m_metaData ) + ", " + std::to_string( m_acquisitionSize );
}

// constexpr int SensorSpec::format2nByte(const Format &format)
//{
//     return s_format2nByte[static_cast<int>(format)];
// }

std::string SensorSpec::to_string( const SensorSpec::MetaData& metaData, bool expand ) {
    std::string str = "";
    if ( expand ) {
        bool first = true;
        for ( const auto& pair : metaData ) {
            if ( first )
                first = false;
            else
                str += "\n";
            str += to_string( pair );
        }
    }
    else {
        str += "[";
        for ( const auto& pair : metaData ) {
            str += to_string( pair ) + ", ";
        }
        str += "]";
    }
    return str;
}

//std::string SensorSpec::to_string( const std::pair<std::string, std::any>& metaData ) {
std::string SensorSpec::to_string( const std::pair<std::string, Any>& metaData ) {
    const auto& name = metaData.first;
    const auto& val  = metaData.second;
    // #ifdef WIN32
    // #else
    std::string str = hub::Any::anyType2string( val ) + " " + name + " = '" +
                      hub::Any::anyValue2string( val ) + "'";
    // #endif
    return str;
}

std::ostream& operator<<( std::ostream& os, const SensorSpec& sensorSpec ) {
    os << sensorSpec.to_string();
    //    os << sensorSpec.m_sensorName << ", " << resolutions2string( sensorSpec.m_resolutions ) <<
    //    ", " os << sensorSpec.m_sensorName << ", " << sensorSpec.m_resolutions << ", "
    //       << SensorSpec::to_string( sensorSpec.m_metaData ) << ", "
    //       << sensorSpec.m_acquisitionSize;
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

// hub::Resolutions SensorSpec::getResolutions() const
//{
//     return m_resolutions;
// }

} // namespace hub
