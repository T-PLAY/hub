#pragma once


#include <algorithm>
#include <any>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "Macros.hpp"
#include "Resolution.hpp"

#if CPLUSPLUS_VERSION == 20
#    include "Map.hpp"
#    include <ranges>
#endif

namespace hub {

///
/// \brief The SensorSpec class
/// describes the sensor in its entirety, like the internal properties of a sensor.
/// Such as the name of the sensor given by the manufacturer and the format/resolution of the measured data.
/// This allows to describe how the data can be visualized and to standardize data that looks similar.
/// Optional information can be added but does not influence the flow communication process.
/// \note The sensor specifications are shared between input and output sensor.
///
class SRC_API SensorSpec
{
  public:
#if CPLUSPLUS_VERSION == 20
    using SensorNameType = std::string_view;
    //    using SensorNameType = std::string;
    using MetaData = Map<std::string, std::any>; // any -> C++17
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
    using MetaData       = std::map<std::string, std::any>; // any -> C++17
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
    explicit CONSTEXPR20 SensorSpec( const SensorNameType& sensorName = "",
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
    static std::string to_string( const std::pair<std::string, std::any>& metaData );

  public:
#if CPLUSPLUS_VERSION == 20
    inline constexpr const SensorNameType& getSensorName() const noexcept;
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
    inline constexpr Resolutions& getResolutions() const noexcept;
    ///
    /// \brief getMetaData
    /// \return
    ///
    inline constexpr MetaData& getMetaData() const noexcept;
    ///
    /// \brief getMetaData
    /// \return
    ///
    inline MetaData& getMetaData() noexcept;
    ///
    /// \brief getAcquisitionSize
    /// \return
    ///
    inline constexpr size_t getAcquisitionSize() const noexcept;

    ///
    /// \brief setMetaData
    /// \param metaData
    ///
    inline void setMetaData( const MetaData& metaData ) noexcept;

  private:
    //    static std::string m_sensorName2;
    SensorNameType m_sensorName;
    Resolutions m_resolutions;
    MetaData m_metaData;

    size_t m_acquisitionSize;
};

} // namespace hub

#include <SensorSpec.inl>