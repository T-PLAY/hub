#include "SensorSpec.hpp"

#include <cassert>
#include <iostream>
// #include <numeric>

//#include "io/Interface.hpp"
#include "Any.hpp"

namespace hub {

#if CPLUSPLUS_VERSION != 20
SensorSpec::SensorSpec( const SensorNameType& sensorName,
                        const Resolutions& resolutions,
                        const MetaData& metaData ) :
    m_sensorName( sensorName ),
    m_resolutions( resolutions ),
    m_metaData( metaData ),
    m_acquisitionSize( res::computeAcquisitionSize( resolutions ) ) {}
#endif

// constexpr SensorSpec::SensorSpec(
//{

//}

// constexpr SensorSpec::SensorSpec( const std::string_view& sensorName,

SensorSpec SensorSpec::operator+( const SensorSpec& sensorSpec ) const {
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

    return SensorSpec( std::move( sensorName ), std::move( resolutions ), std::move( metaData ) );
}

SensorSpec& SensorSpec::operator+=( const SensorSpec& sensorSpec ) {
    if ( m_sensorName == "" ) { *this = sensorSpec; }
    else {
        *this = *this + sensorSpec;
    }
    return *this;
}

bool SensorSpec::operator==( const SensorSpec& sensorSpec ) const {

    return m_sensorName == sensorSpec.m_sensorName && m_resolutions == sensorSpec.m_resolutions &&
           m_acquisitionSize == sensorSpec.m_acquisitionSize &&
           m_metaData.size() == sensorSpec.m_metaData.size();
    // todo any compare
}

std::string SensorSpec::to_string() const {
    return m_sensorName + ", " + hub::res::to_string( m_resolutions ) + ", " +
           SensorSpec::to_string( m_metaData ) + ", " + std::to_string( m_acquisitionSize );
}

bool SensorSpec::isEmpty() const
{
    return m_sensorName == "" && m_resolutions.empty();
}

// constexpr int SensorSpec::format2nByte(const Format &format)
//{
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

// std::string SensorSpec::to_string( const std::pair<std::string, std::any>& metaData ) {
std::string SensorSpec::to_string( const std::pair<std::string, Any>& metaData ) {
    const auto& name = metaData.first;
    const auto& val  = metaData.second;
    // #ifdef WIN32
    // #else
    std::string str = name + " = '" + val.to_string();
    // #endif
    return str;
}

std::ostream& operator<<( std::ostream& os, const SensorSpec& sensorSpec ) {
    os << sensorSpec.to_string();
    return os;
}

// std::string SensorSpec::getSensorName() const
//{
// }

// hub::SensorSpec::MetaData SensorSpec::getMetaData() const
//{
// }

// hub::Resolutions SensorSpec::getResolutions() const
//{
// }

} // namespace hub
