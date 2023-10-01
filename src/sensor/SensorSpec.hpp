#pragma once

#include <algorithm>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "core/Any.hpp"
#include "core/Macros.hpp"
#include "Resolution.hpp"
//#include "Measure.hpp"

// user friendly useless includes
//#include "data/Dof6.hpp"
//#include "data/Mat4.hpp"
//#include "data/Mesh.hpp"
//#include "data/UserData.hpp"

#if CPLUSPLUS_VERSION == 20
#    include "Map.hpp"
#    include <ranges>
#endif

namespace hub {
namespace sensor {

///
/// \brief The SensorSpec class
/// describes the sensor in its entirety, like the internal properties of a sensor.
/// Such as the name of the sensor given by the manufacturer and the format/resolution of the
/// measured data. This allows to describe how the data can be visualized and to standardize data
/// that looks similar. Optional information can be added but does not influence the flow
/// communication process. \note The sensor specifications are shared between input and output
/// sensor.
///
class SRC_API SensorSpec
{
  public:
#if CPLUSPLUS_VERSION == 20
    using SensorNameType = std::string_view;
    using MetaData       = Map<std::string, std::any>; // any -> C++17
#else
    ///
    /// \brief
    /// Type of sensor name
    ///
    using SensorNameType = std::string;

    ///
    /// \brief
    /// Definition of metadata
    ///
    using MetaData = std::map<std::string, Any>;
#endif

    ///
    /// \brief
    /// \param sensorName
    /// [in] Given by the sensor manufacturer or user defined.
    /// \param resolutions
    /// [in] Resolution of the corresponding sensor.
    /// \param metaData
    /// [in] Additional informations of the sensor and acquisition.
    ///
    CONSTEXPR20 SensorSpec( const SensorNameType& sensorName = "",
                                     const Resolutions& resolutions   = {},
                                     const MetaData& metaData         = {} );

    CONSTEXPR20 SensorSpec( const char * sensorName,
                                     const Resolutions& resolutions   = {},
                                     const MetaData& metaData         = {} );

  public:
    ///
    /// \brief operator +
    /// \param sensorSpec
    /// \return
    ///
    SensorSpec operator+( const SensorSpec& sensorSpec ) const;

    ///
    /// \brief operator +=
    /// \param sensorSpec
    /// \return
    ///
    SensorSpec& operator+=( const SensorSpec& sensorSpec );

    ///
    /// \brief operator ==
    /// \param sensorSpec
    /// \return
    ///
    bool operator==( const SensorSpec& sensorSpec ) const;

    ///
    /// \brief operator <<
    /// \param os
    /// \param sensorSpec
    /// \return
    ///
    SRC_API friend std::ostream& operator<<( std::ostream& os, const SensorSpec& sensorSpec );

    ///
    /// \brief to_string
    /// \return
    ///
    std::string to_string() const;

    ///
    /// \brief isEmpty
    /// \return
    ///
    bool isEmpty() const;

  public:
    ///
    /// \brief to_string
    /// \param metaData
    /// \param expand
    /// \return
    ///
    static std::string to_string( const MetaData& metaData, bool expand = false );

    ///
    /// \brief to_string
    /// \param metaData
    /// \return
    ///
    static std::string to_string( const std::pair<std::string, Any>& metaData );

  public:
#if CPLUSPLUS_VERSION == 20
    inline CONSTEXPR const SensorNameType& getSensorName() const noexcept;
#else
    ///
    /// \brief getSensorName
    /// \return
    ///
    inline const std::string& getSensorName() const noexcept;
#endif
    ///
    /// \brief getResolutions
    /// \return
    ///
    inline CONSTEXPR Resolutions& getResolutions() const noexcept;
    ///
    /// \brief getMetaData
    /// \return
    ///
    inline CONSTEXPR MetaData& getMetaData() const noexcept;
    ///
    /// \brief getMetaData
    /// \return
    ///
    inline MetaData& getMetaData() noexcept;
    ///
    /// \brief getAcquisitionSize
    /// \return
    ///
    inline CONSTEXPR size_t getAcquisitionSize() const noexcept;

    ///
    /// \brief setMetaData
    /// \param metaData
    ///

  private:
    SensorNameType m_sensorName;
    Resolutions m_resolutions;
    MetaData m_metaData;

    size_t m_acquisitionSize;
};

///////////////////////////////////////////////// INLINE
/////////////////////////////////////////////////////////////////////////

#if CPLUSPLUS_VERSION == 20
CONSTEXPR20 SensorSpec::SensorSpec( const SensorNameType& sensorName,
                                    const Resolutions& resolutions,
                                    const MetaData& metaData ) :
    m_sensorName( sensorName ),
    m_resolutions( resolutions ),
    m_metaData( metaData ),
    m_acquisitionSize( computeAcquisitionSize( resolutions ) ) {}
#endif

inline CONSTEXPR20 const SensorSpec::SensorNameType& SensorSpec::getSensorName() const noexcept {
    return m_sensorName;
}

inline CONSTEXPR Resolutions& SensorSpec::getResolutions() const noexcept {
    return const_cast<Resolutions&>( m_resolutions );
}

inline CONSTEXPR SensorSpec::MetaData& SensorSpec::getMetaData() const noexcept {
    return const_cast<SensorSpec::MetaData&>( m_metaData );
}

inline SensorSpec::MetaData& SensorSpec::getMetaData() noexcept {
    return m_metaData;
}

inline CONSTEXPR size_t SensorSpec::getAcquisitionSize() const noexcept {
    return m_acquisitionSize;
}

} // namespace sensor
} // namespace hub
