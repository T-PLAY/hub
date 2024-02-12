
#include <thread>

#include <sensor/OutputSensor.hpp>

int main() {

    using Resolution       = hub::format::Dof6;
    const hub::sensor::SensorSpec sensorSpec( FILE_NAME, Resolution() );
    assert( FILE_NAME == "example-OutputSensor_6DOF.cpp" );

    hub::sensor::OutputSensorT<Resolution> outputSensor( sensorSpec, FILE_NAME );
    auto acq                 = outputSensor.acqMsg();
    auto& start              = acq.start();
    auto& end                = acq.end();
    auto& dof6            = acq.get<hub::format::Dof6&>();
    dof6                     = hub::format::Dof6 {};
    //constexpr size_t imgSize = width * height;

    constexpr auto maxFps = 120.0;

    int dec = 0;

    while ( 1 ) {
        const auto startClock = std::chrono::high_resolution_clock::now();

        start = hub::sensor::getClock();
        dof6.x = dec % 100;
        dof6.y = dec % 100;
        dof6.z = dec % 100;
        end = hub::sensor::getClock();
        ++dec;
        //dec += 10;

        // std::cout << "acq: " << acq << std::endl;
        outputSensor << acq;

        const auto endClock = startClock + std::chrono::microseconds( (int)( 1'000'000 / maxFps ) );
        std::this_thread::sleep_until( endClock );
    }

    return 0;
}
