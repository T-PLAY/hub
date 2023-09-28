
#include "test_common.hpp"
#include <catch2/catch_test_macros.hpp>

#include <sensor/InputSensor.hpp>
#include <sensor/OutputSensor.hpp>

// #include <io/Interface.hpp>

TEST_CASE( "OutputSensor test" ) {

    std::vector<char> buff;

    const auto resolution = hub::sensor::Resolution { { 1 }, hub::sensor::Format::BGR8 };
    const hub::sensor::SensorSpec sensorSpec( "sensorName", { resolution } );
    unsigned char data[3] { 1, 2, 3 };
    hub::sensor::OutputSensor outputSensor( sensorSpec, hub::io::Memory<decltype( buff )>( buff ) );

    auto acq = std::move( hub::sensor::Acquisition { 0, 1 } << hub::Measure { data, 3, resolution } );
    outputSensor << acq;
    outputSensor << acq;
    outputSensor << acq;

    hub::sensor::InputSensor inputSensor { hub::io::Memory<decltype( buff )>( buff ) };

    hub::sensor::Acquisition acq2;
    inputSensor >> acq2;
    CHECK( acq == acq2 );

    const auto& input = inputSensor.getInput();
    CHECK( !input.isEnd() );

    auto acqs = inputSensor.getAllAcquisitions();
    CHECK( acqs.size() == 2 );
    CHECK( acqs[0] == acq );
    CHECK( acqs[1] == acq );

    CHECK( input.isEnd() );
}
