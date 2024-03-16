

#include <sensor/OutputSensor.hpp>

/// \file
/// \brief  Main function
/// describes how use a file to init input/output sensor stream
///
int main() {
    /// Comments I would like to be documented in as well

    constexpr auto width  = 512;
    constexpr auto depth  = 512;
    constexpr auto height = 100;

    using Format = hub::format::Density;
    using Resolution = hub::MatrixXD<Format, width, depth, height>;
    using OutputSensor = hub::sensor::OutputSensorT<Resolution>;
    hub::sensor::SensorSpec sensorSpec2( "sensorName", Resolution() );

    OutputSensor outputSensor(sensorSpec2, "streamName");

    auto acq = outputSensor.acqMsg();
    // Acq acq;
    auto [start, end] = acq.clocks();
    auto * densities = acq.get<Format*>();

    for (auto i = 0; i < width; ++i) {
        for (auto j = 0; j < depth; ++j) {
            for (auto k = 0; k < height; ++k) {


            }
        }
    }



    return 0;






    return 0;
}
