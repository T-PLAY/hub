/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright © 2021-2024 Hub. All Rights Reserved.
 * @author Gauthier Bouyjou <bouyjou@toulouse-tech-transfer.com>
 * @date 2024/02/25
 */

#pragma once

#include "OutputSensor_Instance.hpp"

namespace hub {
namespace sensor {
namespace outputSensor {

///
/// \brief The OutputSensor_3D_Density class
///
class OutputSensor_3D_Density : public OutputSensor_Instance
{
  public:
    /// \brief asyncRun
    using OutputSensor_Instance::asyncRun;
    /// \brief OutputSensor_Instance
    using OutputSensor_Instance::OutputSensor_Instance;
    /// \brief run
    using OutputSensor_Instance::run;
    /// \brief stop
    using OutputSensor_Instance::stop;

    ///
    /// \brief side
    ///
    static constexpr int side  = 100;

    ///
    /// \brief width
    ///
    static constexpr int width  = side;

    ///
    /// \brief depth
    ///
    static constexpr int depth  = side;

    ///
    /// \brief height
    ///
    static constexpr int height = side;

    ///
    /// \brief Resolution
    ///
    using Resolution            = hub::MatrixXD<hub::format::Density, width, depth, height>;
    // using Resolution            = hub::MatrixXD<hub::format::RGB8, 640, 480>;

    void init() const override {
        hub::MetaData metaData;
        metaData["author"] = "gauthier";
        m_sensorSpec       = SensorSpec( FILE_NAME_WITHOUT_EXTENSION, Resolution(), metaData );
        m_streamName       = FILE_NAME_WITHOUT_EXTENSION;
        m_inited           = true;
    }

    void routine() override {

        std::unique_ptr<OutputSensor> outputSensor;
        if ( onNewAcq == nullptr ) {
            outputSensor =
                std::make_unique<OutputSensor>( m_sensorSpec, getName(), m_port, m_ipv4 );
        }

        auto acq                  = hub::sensor::make_acquisition( Resolution().getMatrix() );
        auto& start               = acq.start();
        auto& end                 = acq.end();
        auto* density             = acq.get<hub::format::Density*>();
        const auto capacity = acq.getCapacity<hub::format::Density>();
        // constexpr size_t capacity = width * depth * height;
        assert(capacity == width * depth * height);

        constexpr auto maxFps = 40.0;

        int dec = 0;

        while ( m_running ) {
            const auto startClock = std::chrono::high_resolution_clock::now();

            start = hub::sensor::getClock();
            for ( int i = 0; i < width; ++i ) {
                for ( int j = 0; j < depth; ++j ) {
                    for ( int k = 0; k < height; ++k ) {
                        // const auto idx = i * (depth * height) + j * height + k;
                        const auto idx = i + width * ( j + height * k );
                        assert( idx < capacity );
                        // const auto value = ((i + dec) % width) / (float)width ;
                        // const auto value = ((i * j * k + dec) % capacity)  / (float)capacity;
                        const auto value = ((i + dec) % width) / (float)width;
                        // const auto value = ((i * j * k + dec) % capacity) / (float)capacity;
                        assert(0.0 <= value && value <= 1.0);
                        density[idx].value = value;
                    }
                }
            }
            end = hub::sensor::getClock();
            ++dec;
            // dec += 1000;

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
