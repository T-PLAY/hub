#pragma once

#include <algorithm>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

// #include "io/output/Output.hpp"
// #include "io/input/Input.hpp"
#include "Resolution.hpp"
#include "core/Any.hpp"
#include "core/Macros.hpp"
// #include "Measure.hpp"

// user friendly useless includes
// #include "data/Dof6.hpp"
// #include "data/Mat4.hpp"
// #include "data/Mesh.hpp"
// #include "data/UserData.hpp"

#if CPLUSPLUS_VERSION == 20
// #    include "Map.hpp"
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
    // #if CPLUSPLUS_VERSION == 20
    //     using SensorNameType = std::string_view;
    //     using SensorNameType = std::string;
    //     using MetaData       = Map<std::string, std::any>; // any -> C++17
//    using MetaData = std::pair<std::string, Any>;
    using MetaData = std::map<std::string, Any>; // any -> C++17
                                                 // #else
    //     ///
    //     /// \brief
    //     /// Type of sensor name
    //     ///
    //     using SensorNameType = std::string;

    //    ///
    //    /// \brief
    //    /// Definition of metadata
    //    ///
    //    using MetaData = std::map<std::string, Any>;
    // #endif

    ///
    /// \brief
    /// \param sensorName
    /// [in] Given by the sensor manufacturer or user defined.
    /// \param resolutions
    /// [in] Resolution of the corresponding sensor.
    /// \param metaData
    /// [in] Additional informations of the sensor and acquisition.
    ///
    SensorSpec( const std::string& sensorName,
                         const Resolutions& resolutions,
                         const MetaData& metaData       = {} );

    SensorSpec( const char* sensorName,
                         const Resolutions& resolutions,
                         const MetaData& metaData       = {} );

//    SensorSpec(Input & input)
//        : m_sensorName(input.get<decltype(m_sensorName)>())
////        , m_resolutions(input.get<decltype(m_resolutions)>())
//        , m_metaData(input.get<decltype(m_metaData)>())
//    {
////        input.read( m_sensorName );
////        input.read( m_resolutions );
////        input.read( m_metaData );
//    }

    SensorSpec() = delete;

  public:
    //    const unsigned char* getData() const { return nullptr; }
    //    constexpr std::string typeName() const { return "SensorSpec"; }
    //    size_t getSize() const { return 0; }

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
    bool isEnd() const;


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
    inline CONSTEXPR const std::string& getSensorName() const noexcept;
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
    /// \brief getMetaDatas
    /// \return
    ///
    inline CONSTEXPR MetaData& getMetaDatas() const noexcept;
    ///
    /// \brief getMetaDatas
    /// \return
    ///
    inline MetaData& getMetaDatas() noexcept;
    ///
    /// \brief getAcquisitionSize
    /// \return
    ///
    inline CONSTEXPR Size_t getAcquisitionSize() const noexcept;

    ///
    /// \brief setMetaData
    /// \param metaData
    ///

    //    template <typename Output>
    //    friend Output& operator<<( Output& output, const SensorSpec& sensorSpec );

    //    template <typename Input>
    //    friend Input& operator>>( Input& input, SensorSpec& sensorSpec );

//  private:
//    template <typename Input>
//    void read( Input& input ) {
//        //        input.read( m_format );
//        input.read( m_sensorName );
//        input.read( m_resolutions );
//        input.read( m_metaData );
//    }

//    template <typename Output>
//    void write( Output& output ) const {
//        //        output.write( m_format );
//        output.write( m_sensorName );
//        output.write( m_resolutions );
//        output.write( m_metaData );
//    }

  private:
    std::string m_sensorName;
    Resolutions m_resolutions;
    MetaData m_metaData;

    //    size_t m_acquisitionSize;
};

///////////////////////////////////////////////// INLINE
/////////////////////////////////////////////////////////////////////////

// #if CPLUSPLUS_VERSION == 20
// SensorSpec::SensorSpec( const SensorNameType& sensorName,
//                                     const Resolutions& resolutions,
//                                     const MetaData& metaData ) :
//     m_sensorName( sensorName ),
//     m_resolutions( resolutions ),
//     m_metaData( metaData ),
//     m_acquisitionSize( resolution::computeAcquisitionSize( resolutions ) ) {}
// #endif

inline CONSTEXPR20 const std::string& SensorSpec::getSensorName() const noexcept {
    return m_sensorName;
}

inline CONSTEXPR Resolutions& SensorSpec::getResolutions() const noexcept {
//    return m_resolutions;
    return const_cast<Resolutions&>( m_resolutions );
}

inline CONSTEXPR SensorSpec::MetaData& SensorSpec::getMetaDatas() const noexcept {
    return const_cast<SensorSpec::MetaData&>( m_metaData );
}

inline SensorSpec::MetaData& SensorSpec::getMetaDatas() noexcept {
    return m_metaData;
}

inline constexpr Size_t SensorSpec::getAcquisitionSize() const noexcept {
    return sensor::nByte( m_resolutions );
    //     return m_acquisitionSize;
}

} // namespace sensor
} // namespace hub
