/*******************************************************************************
* Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
* Please visit https://www.irit.fr/tplay/.
*
* All rights reserved.
* This code belongs to tplay/hub project (https://github.com/T-PLAY/hub).
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*
* Initial Contributors:
*   - Nicolas Mellado (IRIT)
*   - Gauthier Bouyjou (IRIT)
*******************************************************************************/

#pragma once

#include <algorithm>
#include <map>

//// #include "io/output/Output.hpp"
//// #include "io/input/Input.hpp"
#include "core/Base.hpp"
#include "core/Any.hpp"
#include "core/MetaData.hpp"
#include "core/Matrix.hpp"
#include "Acquisition.hpp"

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
    ///
    /// \brief
    /// \param sensorName
    /// [in] Given by the sensor manufacturer or user defined.
    /// \param resolution
    /// [in] Resolution of the corresponding sensor.
    /// \param metaData
    /// [in] Additional informations of the sensor and acquisition.
    ///
    SensorSpec( const std::string& sensorName,
                const Matrix& resolution,
                const MetaData& metaData = {} ) :
        m_sensorName { sensorName }, m_resolution { resolution.clone() }, m_metaData { metaData } {}

    ///
    /// \brief SensorSpec
    /// \param sensorName
    /// \param resolution
    /// \param metaData
    ///
    template <class Resolution>
    SensorSpec( const std::string& sensorName,
                const Resolution& resolution,
                const MetaData& metaData = {} ) :
        m_sensorName { sensorName }, m_metaData { metaData } {
        if constexpr ( isMatrix<Resolution> ) { m_resolution = Resolution().getMatrix(); }
        else {
            m_resolution = make_matrix<Resolution>();
        }
    }

    ///
    /// \brief toString
    /// \return
    ///
    auto toString() const {
        return "'" + m_sensorName + "' " + m_resolution.toString() + " " +
               hub::to_string( m_metaData );
    }

    ///
    /// \brief operator ==
    /// \param other
    /// \return
    ///
    bool operator==( const SensorSpec& other ) const {
        return m_sensorName == other.m_sensorName && m_resolution == other.m_resolution &&
               m_metaData == other.m_metaData;
    }


    SensorSpec() = default;

    ///
    /// \brief SensorSpec
    /// \param sensorSpec
    ///
    SensorSpec( const SensorSpec& sensorSpec ) :
        m_sensorName { sensorSpec.m_sensorName },
        m_resolution { sensorSpec.m_resolution.clone() },
        m_metaData { sensorSpec.m_metaData } {};

    ///
    /// \brief operator =
    /// \param sensorSpec
    /// \return
    ///
    SensorSpec& operator=( const SensorSpec& sensorSpec ) {
        m_sensorName = sensorSpec.m_sensorName;
        m_resolution = sensorSpec.m_resolution.clone();
        m_metaData   = sensorSpec.m_metaData;
        return *this;
    }

    ///
    /// \brief write
    /// \param serializer
    ///
    void write( Serializer& serializer ) const {
        serializer.writeAll( m_sensorName, m_resolution, m_metaData );
    }

    ///
    /// \brief read
    /// \param serializer
    ///
    void read( Serializer& serializer ) {
        serializer.readAll( m_sensorName, m_resolution, m_metaData );
    }

    ///
    /// \brief operator +
    /// \param other
    /// \return
    ///
    SensorSpec operator+( const SensorSpec& other ) const {

        std::string sensorName;
        Matrix resolution;
        MetaData metaData;

        sensorName = m_sensorName + " + " + other.m_sensorName;

        resolution = m_resolution | other.m_resolution;

        metaData = m_metaData;
        metaData.insert( other.m_metaData.begin(), other.m_metaData.end() );

        metaData.erase( "parent" );

        return SensorSpec(
            std::move( sensorName ), std::move( resolution ), std::move( metaData ) );
    }

    ///
    /// \brief operator +=
    /// \param other
    /// \return
    ///
    SensorSpec& operator+=( const SensorSpec& other ) {
        if ( m_sensorName == "" ) { *this = other; }
        else {
            *this = *this + other;
        }
        return *this;
    }

    ///
    /// \brief getSensorName
    /// \return
    ///
    const std::string & getSensorName() const { return m_sensorName; }

    ///
    /// \brief getResolution
    /// \return
    ///
    const Matrix& getResolution() const { return m_resolution; }

    ///
    /// \brief getMetaData
    /// \return
    ///
    const MetaData& getMetaData() const { return m_metaData; }

    ///
    /// \brief getMetaData
    /// \return
    ///
    MetaData& getMetaData() { return m_metaData; }

    ///
    /// \brief setResolution
    /// \param newResolution
    ///
    void setResolution( Matrix&& newResolution ) { m_resolution = std::move( newResolution ); }

    ///
    /// \brief setResolution
    /// \param newResolution
    ///
    void setResolution( const Matrix& newResolution ) { m_resolution = newResolution.clone(); }

    ///
    /// \brief dataSize
    /// \return
    ///
    Size_t dataSize() const { return 2 * sizeof( hub::sensor::Clock ) + m_resolution.size(); };

  private:
    std::string m_sensorName;
    Matrix m_resolution;
    MetaData m_metaData;
};

} // namespace sensor
} // namespace hub
