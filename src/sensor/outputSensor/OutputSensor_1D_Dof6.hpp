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
*   - Gauthier Bouyjou (IRIT, Toulouse Tech Transfer)
*******************************************************************************/

#pragma once

#include "OutputSensor_Instance.hpp"

namespace hub {
namespace sensor {
namespace outputSensor {

///
/// \brief The OutputSensor_1D_Dof6 class
///
class OutputSensor_1D_Dof6 : public OutputSensor_Instance
{
  public:
    using OutputSensor_Instance::asyncRun;
    using OutputSensor_Instance::OutputSensor_Instance;
    using OutputSensor_Instance::run;
    using OutputSensor_Instance::stop;

    ///
    /// \brief Resolution
    ///
    using Resolution = hub::format::Dof6;

    ///
    /// \brief init
    ///
    void init() const override {
        hub::MetaData metaData;
        metaData["author"] = "gauthier";
        m_sensorSpec       = SensorSpec( FILE_NAME_WITHOUT_EXTENSION, Resolution(), metaData );
        m_streamName       = FILE_NAME_WITHOUT_EXTENSION;
        m_inited           = true;
    }

    ///
    /// \brief routine
    ///
    void routine() override {

        std::unique_ptr<OutputSensor> outputSensor;
        if ( onNewAcq == nullptr ) {
            outputSensor =
                std::make_unique<OutputSensor>( m_sensorSpec, m_streamName, m_port, m_ipv4 );
        }
        auto acq    = hub::sensor::make_acquisition( hub::make_matrix<Resolution>() );
        auto& start = acq.start();
        auto& end   = acq.end();
        auto& dof6  = acq.get<hub::format::Dof6&>();
        dof6        = hub::format::Dof6 {};

        constexpr auto maxFps = 120.0;

        int dec = 0;

        while ( m_running ) {
            const auto startClock = std::chrono::high_resolution_clock::now();

            start  = hub::sensor::getClock();
            dof6.x = dec % 100;
            dof6.y = dec % 100;
            dof6.z = dec % 100;
            end    = hub::sensor::getClock();
            ++dec;

            if ( onNewAcq ) { onNewAcq( m_streamName, acq ); }
            else { *outputSensor << acq; }

            const auto endClock =
                startClock + std::chrono::microseconds( (int)( 1'000'000 / maxFps ) );
            std::this_thread::sleep_until( endClock );
        }
    }

  private:
};

} // namespace outputSensor
} // namespace sensor
} // namespace hub
