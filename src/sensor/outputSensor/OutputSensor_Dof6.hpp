
#pragma once

#include "OutputSensor_Instance.hpp"

namespace hub {
namespace sensor {
namespace outputSensor {

class OutputSensor_Dof6 : public OutputSensor_Instance
{
  public:
    using OutputSensor_Instance::run;
    using OutputSensor_Instance::asyncRun;
    using OutputSensor_Instance::stop;
    using OutputSensor_Instance::OutputSensor_Instance;



    void routine() override {
        using Resolution = hub::format::Dof6;
        const hub::sensor::SensorSpec sensorSpec( FILE_NAME, Resolution() );
        assert( FILE_NAME == "OutputSensor_Dof6.hpp" );

        hub::sensor::OutputSensorT<Resolution> outputSensor( sensorSpec, FILE_NAME, m_port, m_ipv4 );
        auto acq    = outputSensor.acqMsg();
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

            outputSensor << acq;

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
