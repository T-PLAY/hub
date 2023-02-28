#pragma once

#include "SensorSpec.hpp"

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

inline CONSTEXPR20 const SensorSpec::SensorNameType& SensorSpec::getSensorName() const noexcept
{
    return m_sensorName;
}

inline constexpr Resolutions& SensorSpec::getResolutions() const noexcept {
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

inline void SensorSpec::setMetaData( const MetaData& metaData ) noexcept {
    m_metaData = metaData;
}

} // namespace hub
