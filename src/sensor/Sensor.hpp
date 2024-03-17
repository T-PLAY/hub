/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright © 2021-2024 Hub. All Rights Reserved.
 * @author Gauthier Bouyjou <gauthierbouyjou@aol.com>
 * @date 2023/09/28
 */

#pragma once

#include "Acquisition.hpp"
#include "SensorSpec.hpp"

namespace hub {
namespace sensor {

///
/// \brief The Sensor class
/// describes what a sensor can be, it's defined from the constant specifities of a sensor
/// and uses a communication bus to transmit the data.
/// \note Not user instantiable class.
///
class SRC_API Sensor
{
  protected:
    ///
    /// \brief Sensor
    /// \param sensorSpec
    /// [in] user defined sensor spec
    ///
    Sensor() = default;

    ///
    /// \brief Sensor
    /// \param sensorSpec
    ///
    explicit Sensor( const SensorSpec& sensorSpec ) : m_spec { sensorSpec } {}

    Sensor( const Sensor& )            = delete;
    Sensor( Sensor& )                  = delete;
    Sensor& operator=( const Sensor& ) = delete;
    Sensor& operator=( Sensor& )       = delete;

    ///
    /// \brief acqMsg
    /// \return
    ///
    // Acquisition acqMsg() const { return make_acquisition( m_spec.getResolution() ); }

  public:
    ///
    /// \brief getSpec
    /// \return
    ///
    const SensorSpec& getSpec() const { return m_spec; }

  protected:
    ///
    /// \brief m_spec
    ///
    SensorSpec m_spec;
};

/////////////////////////////////////////////////// INLINE
///////////////////////////////////////////////////////////////////////////

} // namespace sensor
} // namespace hub
