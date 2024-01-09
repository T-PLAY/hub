#pragma once

#include <algorithm>
#include <map>
// #include <stdexcept>
// #include <string>
// #include <vector>

//// #include "io/output/Output.hpp"
//// #include "io/input/Input.hpp"
// #include "Resolution.hpp"
// #include "Resolutions.hpp"
#include "core/Any.hpp"
#include "core/Macros.hpp"
#include "core/MetaData.hpp"

//// #include "Measure.hpp"

//// user friendly useless includes
//// #include "data/Dof6.hpp"
//// #include "data/Mat4.hpp"
//// #include "data/Mesh.hpp"
//// #include "data/UserData.hpp"

// #if CPP_VERSION == 20
//// #    include "Map.hpp"
// #    include <ranges>
// #endif

#include "core/Matrix.hpp"

#include "Acquisition.hpp"

namespace hub {
namespace sensor {

// using Resolution = Matrix;

//     using MetaData       = Map<std::string, std::any>; // any -> C++17
//    using MetaData = std::pair<std::string, Any>;
// using MetaData = std::map<std::string, Any>; // any -> C++17

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
    // #if CPP_VERSION == 20
    //     using SensorNameType = std::string_view;
    //     using SensorNameType = std::string;
    // #else
    // static std::string to_string( const MetaData& metaData, bool expand = false );
    // static std::string to_string( const std::pair<std::string, Any>& metaData );
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
                const Matrix& resolution,
                const MetaData& metaData = {} ) :
        m_sensorName { sensorName }, m_resolution { resolution.clone() }, m_metaData { metaData } {}

    template <class Resolution>
    SensorSpec( const std::string& sensorName,
                const Resolution& resolution,
                const MetaData& metaData = {} ) :
        m_sensorName { sensorName },
        // m_resolution { resolution.getMatrix() },
        m_metaData { metaData } {
        if constexpr ( isMatrix<Resolution> ) { m_resolution = Resolution().getMatrix(); }
        else { m_resolution = make_matrix<Resolution>(); }
    }

    // SensorSpec( const std::string& sensorName, const MetaData& metaData = {} ) :
    // m_sensorName { sensorName }, m_metaData { metaData } {}

    //    SensorSpec(const SensorSpec & sensorSpec) = default;

    auto toString() const {
        return "'" + m_sensorName + "' " + m_resolution.toString() + " " + hub::to_string( m_metaData );
        //        return sensorName + " " + resolution.toString();
    }

    bool operator==( const SensorSpec& other ) const {
        return m_sensorName == other.m_sensorName && m_resolution == other.m_resolution &&
               m_metaData == other.m_metaData;
    }
    //    SensorSpec( const char* m_sensorName,
    //                const Resolution& resolution = {},
    //                const MetaData& metaData = {} );

    //    SensorSpec(Input & input)
    //        : m_sensorName(input.get<decltype(m_sensorName)>())
    ////        , m_resolutions(input.get<decltype(m_resolutions)>())
    //        , m_metaData(input.get<decltype(m_metaData)>())
    //    {
    ////        input.read( m_sensorName );
    ////        input.read( m_resolutions );
    ////        input.read( m_metaData );
    //    }

    SensorSpec() = default;
    //    SensorSpec(const SensorSpec & sensorSpec) = default;

    SensorSpec( const SensorSpec& sensorSpec ) :
        m_sensorName { sensorSpec.m_sensorName },
        m_resolution { sensorSpec.m_resolution.clone() },
        m_metaData { sensorSpec.m_metaData } {};

    SensorSpec& operator=( const SensorSpec& sensorSpec ) {
        m_sensorName = sensorSpec.m_sensorName;
        m_resolution = sensorSpec.m_resolution.clone();
        m_metaData   = sensorSpec.m_metaData;
        return *this;
    }

    //    SensorSpec() = delete;

    //  public:

    void write( Serializer& serializer ) const {
        // std::cout << "[SensorSpec] write(Serializer&) : " << *this << std::endl;
        serializer.writeAll( m_sensorName, m_resolution, m_metaData );
        // serializer.write(m_sensorName);
    }
    void read( Serializer& serializer ) {
        // std::cout << "[SensorSpec] read(Serializer&) : " << *this << std::endl;
        serializer.readAll( m_sensorName, m_resolution, m_metaData );
        // serializer.read(m_sensorName);
    }
    //        static constexpr auto serialize( auto& archive, auto& self ) {
    //            assert(false);
    //            return archive( self.m_sensorName, self.m_resolution, self.m_metaData );
    //            return archive( self.m_sensorName, self.resolution );
    //        }

    SensorSpec operator+( const SensorSpec& other ) const {

        std::string sensorName;
        Matrix resolution;
        MetaData metaData;

        sensorName = m_sensorName + " + " + other.m_sensorName;

        // resolution = make_matrix( m_resolution, other.m_resolution );
        resolution = m_resolution | other.m_resolution;
        //        resolution = m_resolution.clone();
        //        resolution.insert(
        //            resolution.end(), other.m_resolutions.begin(),
        //            other.m_resolutions.end() );
        //        resolution.push_back(other.m_resolution);

        metaData = m_metaData;
        metaData.insert( other.m_metaData.begin(), other.m_metaData.end() );

        metaData.erase( "parent" );

        return SensorSpec(
            std::move( sensorName ), std::move( resolution ), std::move( metaData ) );
    }

    SensorSpec& operator+=( const SensorSpec& other ) {
        if ( m_sensorName == "" ) { *this = other; }
        else { *this = *this + other; }
        return *this;
    }

    //    void serialize(Serializer & serializer) {
    //        serializer(m_sensorName, resolution, metaData);
    //    }

    std::string getSensorName() const { return m_sensorName; }
    const Matrix& getResolution() const { return m_resolution; }
    const MetaData& getMetaData() const { return m_metaData; }
    MetaData& getMetaData() { return m_metaData; }

    void setResolution( Matrix&& newResolution ) { m_resolution = std::move( newResolution ); }
    void setResolution( const Matrix& newResolution ) { m_resolution = newResolution.clone(); }

    Size_t dataSize() const { return 2 * sizeof( hub::sensor::Clock ) + m_resolution.size(); };

  private:
    std::string m_sensorName;
    Matrix m_resolution;
    MetaData m_metaData;

    //    size_t m_acquisitionSize;
};
//    const unsigned char* getData() const { return nullptr; }
//    constexpr std::string typeName() const { return "SensorSpec"; }
//    size_t getSize() const { return 0; }

//    ///
//    /// \brief operator +
//    /// \param sensorSpec
//    /// \return
//    ///
//    SensorSpec operator+( const SensorSpec& sensorSpec ) const;

//    ///
//    /// \brief operator +=
//    /// \param sensorSpec
//    /// \return
//    ///
//    SensorSpec& operator+=( const SensorSpec& sensorSpec );

//    ///
//    /// \brief operator ==
//    /// \param sensorSpec
//    /// \return
//    ///
//    bool operator==( const SensorSpec& sensorSpec ) const;

//    ///
//    /// \brief operator <<
//    /// \param os
//    /// \param sensorSpec
//    /// \return
//    ///
//    SRC_API friend std::ostream& operator<<( std::ostream& os, const SensorSpec& sensorSpec );

//    ///
//    /// \brief to_string
//    /// \return
//    ///
//    std::string to_string() const;

//    ///
//    /// \brief isEmpty
//    /// \return
//    ///
//    bool isEnd() const;

//  public:
//    ///
//    /// \brief to_string
//    /// \param metaData
//    /// \param expand
//    /// \return
//    ///

//    ///
//    /// \brief to_string
//    /// \param metaData
//    /// \return
//    ///

//  public:
// #if CPP_VERSION == 20
//    inline CONSTEXPR const std::string& getSensorName() const noexcept;
// #else
//    ///
//    /// \brief getSensorName
//    /// \return
//    ///
//    inline const std::string& getSensorName() const noexcept;
// #endif
//    ///
//    /// \brief getResolutions
//    /// \return
//    ///
//    inline CONSTEXPR Resolutions& getResolutions() const noexcept;

//    ///
//    /// \brief getMetaDatas
//    /// \return
//    ///
//    inline CONSTEXPR MetaData& getMetaDatas() const noexcept;
//    ///
//    /// \brief getMetaDatas
//    /// \return
//    ///
//    inline MetaData& getMetaDatas() noexcept;
//    ///
//    /// \brief getAcquisitionSize
//    /// \return
//    ///
//    inline CONSTEXPR Size_t getAcquisitionSize() const noexcept;

//    ///
//    /// \brief setMetaData
//    /// \param metaData
//    ///

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

/////////////////////////////////////////////////// INLINE
///////////////////////////////////////////////////////////////////////////

//// #if CPP_VERSION == 20
//// SensorSpec::SensorSpec( const SensorNameType& sensorName,
////                                     const Resolutions& resolution,
////                                     const MetaData& metaData ) :
////     m_sensorName( sensorName ),
////     m_resolutions( resolutions ),
////     m_metaData( metaData ),
////     m_acquisitionSize( resolution::computeAcquisitionSize( resolutions ) ) {}
//// #endif

// inline CONSTEXPR20 const std::string& SensorSpec::getSensorName() const noexcept {
//     return m_sensorName;
// }

// inline CONSTEXPR Resolutions& SensorSpec::getResolutions() const noexcept {
////    return m_resolutions;
//    return const_cast<Resolutions&>( m_resolutions );
//}

// inline CONSTEXPR SensorSpec::MetaData& SensorSpec::getMetaDatas() const noexcept {
//     return const_cast<SensorSpec::MetaData&>( m_metaData );
// }

// inline SensorSpec::MetaData& SensorSpec::getMetaDatas() noexcept {
//     return m_metaData;
// }

// inline constexpr Size_t SensorSpec::getAcquisitionSize() const noexcept {
//     return sensor::nByte( m_resolutions );
//     //     return m_acquisitionSize;
// }

} // namespace sensor
} // namespace hub
