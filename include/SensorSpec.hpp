#pragma once

#include <algorithm>
#include <any>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>
// #include <exception>

#include "Macros.hpp"
#include "Resolution.hpp"
#if CPLUSPLUS_VERSION == 20
#    include "Map.hpp"
#    include <ranges>
#endif

// #if ( __cplusplus >= 202001L )
// #else
// #    error "only C++20 accepted"
// #endif

namespace hub {

///
/// \brief The SensorSpec class
/// describes the sensor in its entirety, like the internal properties of a sensor.
/// Such as the name of the sensor given by the manufacturer and the format/resolution of the measured data.
/// This allows to describe how the data can be visualized and to standardize data that looks similar.
/// Optional information can be added but does not influence the flow communication process.
/// \note The sensor specifications are shared between input and output sensor.
///
// template <int NDim>
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

    //    using MetaData = Map<std::string, std::any>; // any -> C++17
    //    using MetaData = std::vector<std::pair<std::string, std::any>>;
    //    using MetaData = std::vector<std::pair<std::string, std::any>>;

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
    //    m_sensorName( sensorName ),
    //    m_resolutions( resolutions ),
    //    m_metaData( metaData ),
    //    m_acquisitionSize( computeAcquisitionSize( resolutions ) ) {}

    //    constexpr SensorSpec( const std::string& sensorName,
    //                          const Resolutions& resolutions) noexcept :
    //        m_sensorName( sensorName ), m_resolutions( resolutions ) {};
    //    //    ~SensorSpec() = default;

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
    //    SRC_API friend std::ostream& operator<<( std::ostream& os, const Format& format );
    ///
    /// \brief operator <<
    /// \param os
    /// \param sensorSpec
    /// \return
    ///
    SRC_API friend std::ostream& operator<<( std::ostream& os, const SensorSpec& sensorSpec );

  public:
    //    static inline CONSTEXPR20 size_t computeAcquisitionSize( const Resolutions& resolutions )
    //    noexcept; static inline CONSTEXPR20 size_t computeAcquisitionSize( const Resolution&
    //    resolution ) noexcept; static inline constexpr int format2nByte( const Format& format )
    //    noexcept; static inline constexpr bool isInterpolable( const Format& format ) noexcept;

    //    static std::string dims2string( const Dims& dims );
    //    static std::string format2string( const Format& format );
    //    static std::string resolutions2string( const Resolutions& resolutions );
    ///
    /// \brief metaData2string
    /// \param metaData
    /// \param expand
    /// \return
    ///
    static std::string metaData2string( const MetaData& metaData, bool expand = false );

    ///
    /// \brief metaData2string
    /// \param metaData
    /// \return
    ///
    static std::string metaData2string( const std::pair<std::string, std::any>& metaData );

  public:
    //    inline const std::string& getSensorName() const noexcept;
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

    ///////////////////////////////////////////////////////////////////////////////////////////////

    //  private:
    //    static inline CONSTEXPR20 size_t computeAcquisitionSize( Format format, const Dims& dims )
    //    noexcept;

  private:
    //    static std::string m_sensorName2;
    SensorNameType m_sensorName;
    Resolutions m_resolutions;
    MetaData m_metaData;

    size_t m_acquisitionSize;
};

} // namespace hub

#include <SensorSpec.inl>
