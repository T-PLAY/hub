#pragma once
#include "SensorSpec.hpp"

// #include <array>
#include <numeric>

namespace hub {

#if CPLUSPLUS_VERSION == 20
CONSTEXPR20 SensorSpec::SensorSpec( const SensorNameType& sensorName,
                                    const Resolutions& resolutions,
                                    const MetaData& metaData ) :
    m_sensorName( sensorName ),
    m_resolutions( resolutions ),
    m_metaData( metaData ),
    m_acquisitionSize( computeAcquisitionSize( resolutions ) ) {}
#endif

// getters

// std::string tmp = "hello";

// inline const std::string &SensorSpec::getSensorName() const noexcept
//{
////    return std::string("hello");
//    return tmp; // todo
//}

// #if CPLUSPLUS_VERSION == 20
// inline CONSTEXPR20 SensorSpec::SensorNameType SensorSpec::getSensorName() const noexcept
// #else
inline CONSTEXPR20 const SensorSpec::SensorNameType& SensorSpec::getSensorName() const noexcept
// #endif
{
    return m_sensorName;
}

inline constexpr Resolutions& SensorSpec::getResolutions() const noexcept {
    //    return m_resolutions;
    //    return m_resolutions;
    return const_cast<Resolutions&>( m_resolutions );
}

inline constexpr SensorSpec::MetaData& SensorSpec::getMetaData() const noexcept {
    return const_cast<SensorSpec::MetaData&>( m_metaData );
}

inline SensorSpec::MetaData& SensorSpec::getMetaData() noexcept {
    return m_metaData;
}

inline constexpr size_t SensorSpec::getAcquisitionSize() const noexcept {
    return m_acquisitionSize;
}

// setters

void SensorSpec::setMetaData( const MetaData& metaData ) noexcept {
    m_metaData = metaData;
}

} // namespace hub
