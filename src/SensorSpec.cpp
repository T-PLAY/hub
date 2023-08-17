#include "SensorSpec.hpp"

#include <cassert>
#include <iostream>
// #include <numeric>
#include <cmath>

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

SensorSpec::SensorSpec(const char *sensorName, const Resolutions &resolutions, const MetaData &metaData) :
    m_sensorName( sensorName ),
    m_resolutions( resolutions ),
    m_metaData( metaData ),
    m_acquisitionSize( res::computeAcquisitionSize( resolutions ) ) {}

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

// source : https://www.mbeckler.org/blog/?p=114
// Prints to the provided buffer a nice number of bytes (KB, MB, GB, etc)
void pretty_bytes( char* buf, uint64_t bytes ) {
    const char* suffixes[7];
    suffixes[0]  = "B";
    suffixes[1]  = "KB";
    suffixes[2]  = "MB";
    suffixes[3]  = "GB";
    suffixes[4]  = "TB";
    suffixes[5]  = "PB";
    suffixes[6]  = "EB";
    uint64_t s   = 0; // which suffix to use
    double count = bytes;
    while ( count >= 1024 && s < 7 ) {
        s++;
        count /= 1024;
    }
    if ( count - floor( count ) == 0.0 )
#ifdef WIN32
        sprintf_s( buf, 80, "%d %s", (int)count, suffixes[s] );
#else
        sprintf( buf, "%d %s", (int)count, suffixes[s] );
#endif
    else
#ifdef WIN32
        sprintf_s( buf, 80, "%.1f %s", count, suffixes[s] );
#else
        sprintf( buf, "%.1f %s", count, suffixes[s] );
#endif
}

std::string SensorSpec::to_string() const {
    char acqSizeStr[80];
    pretty_bytes( acqSizeStr, m_acquisitionSize );
    ////        acqSizeStr += std::to_string((int)(m_acquisitionSize / std::pow(10, 3 * i)) %
    /// 1'000); /        acqSizeStr += std::to_string(hundred); /        else if (i == 0) { /
    /// std::stringstream ss; /            ss << std::fixed << std::setprecision(1) << hundred /
    /// 1000.0; /            acqSizeStr += ss.str(); /            acqSizeStr +=
    /// std::to_string(hundred); /        }

    return "'" + m_sensorName + "', " + hub::res::to_string( m_resolutions ) + ", " +
           SensorSpec::to_string( m_metaData ) + ", " + acqSizeStr;
}

bool SensorSpec::isEmpty() const {
    return m_sensorName == "" && m_resolutions.empty();
}

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

std::string SensorSpec::to_string( const std::pair<std::string, Any>& metaData ) {
    const auto& name = metaData.first;
    const auto& val  = metaData.second;
    std::string str  = name + " : " + val.to_string();
    return str;
}

std::ostream& operator<<( std::ostream& os, const SensorSpec& sensorSpec ) {
    os << sensorSpec.to_string();
    return os;
}

} // namespace hub

