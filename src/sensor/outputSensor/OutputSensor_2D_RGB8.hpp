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
/// \brief The OutputSensor_2D_RGB8 class
/// mimic camera sensor
///
class OutputSensor_2D_RGB8 : public OutputSensor_Instance
{
  public:
    using OutputSensor_Instance::asyncRun;
    using OutputSensor_Instance::OutputSensor_Instance;
    using OutputSensor_Instance::run;
    using OutputSensor_Instance::stop;

    ///
    /// \brief width
    ///
    static constexpr int width  = 640;

    ///
    /// \brief height
    ///
    static constexpr int height = 480;

    ///
    /// \brief Resolution
    ///
    using Resolution            = hub::MatrixXD<hub::format::RGB8, width, height>;
    // using Resolution            = hub::MatrixXD<hub::format::RGB8, 640, 480>;

    ///
    /// \brief init
    ///
    void init() const override {
        hub::MetaData metaData;
        metaData["author"] = "gauthier";
        metaData["parent"] = "OutputSensor_1D_Dof6";
        m_sensorSpec       = SensorSpec( FILE_NAME_WITHOUT_EXTENSION, Resolution(), metaData );
        m_streamName       = FILE_NAME_WITHOUT_EXTENSION;
        m_inited           = true;
    }

    ///
    /// \brief routine
    ///
    void routine() override {
    // constexpr int width  = 640;
    // constexpr int height = 480;

        // hub::sensor::OutputSensorT<Resolution> outputSensor( sensorSpec, FILE_NAME );
        std::unique_ptr<OutputSensor> outputSensor;
        if ( onNewAcq == nullptr ) {
            outputSensor =
                std::make_unique<OutputSensor>( m_sensorSpec, getName(), m_port, m_ipv4 );
        }

        // auto acq                 = outputSensor.acqMsg();
        // auto acq = hub::sensor::make_acquisition(hub::make_matrix<Resolution>());
        auto acq                 = hub::sensor::make_acquisition( Resolution().getMatrix() );
        auto& start              = acq.start();
        auto& end                = acq.end();
        auto* imgData            = acq.get<hub::format::RGB8*>();
        constexpr size_t imgSize = width * height;

        constexpr auto maxFps = 120.0;

        int dec          = 0;
        const auto third = width / 3;

        while ( m_running ) {
            const auto startClock = std::chrono::high_resolution_clock::now();

            start = hub::sensor::getClock();
            for ( int i = 0; i < height; ++i ) {
                for ( int j = 0; j < width; ++j ) {
                    const auto idx = i * width + j;
                    assert( idx < imgSize );
                    // imgData[idx].r = ( i + j + dec ) / 10 % 128;
                    // imgData[idx].g = ( i + j + dec ) / 10 % 128;
                    // imgData[idx].b = ( i + j + dec ) / 10 % 128;
                    imgData[idx].r = ( i + j + dec ) % 128;
                    imgData[idx].g = ( i + j + dec ) % 128;
                    imgData[idx].b = ( i + j + dec ) % 128;
                    if ( j < third ) { imgData[idx].r += 128; }
                    else if ( j < 2 * third ) {
                        imgData[idx].r += 128;
                        imgData[idx].g += 128;
                        imgData[idx].b += 128;
                    }
                    else { imgData[idx].b += 128; }
                }
            }
            end = hub::sensor::getClock();
            ++dec;

            // outputSensor << acq;
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
