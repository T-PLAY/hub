
#include "test-common.hpp"
#include <catch2/catch_test_macros.hpp>

#include <InputSensor.hpp>
#include <OutputSensor.hpp>

// #include <io/Interface.hpp>

TEST_CASE( "OutputSensor test" ) {

    std::vector<char> buff;

    const auto resolution = hub::Resolution { { 1 }, hub::Format::BGR8 };
    const hub::SensorSpec sensorSpec( "sensorName", { resolution } );
    unsigned char data[3] { 1, 2, 3 };
    hub::OutputSensor outputSensor( sensorSpec, hub::io::Memory<decltype( buff )>( buff ) );

    auto acq = std::move( hub::Acquisition { 0, 1 } << hub::data::Measure { data, 3, resolution } );
    outputSensor << acq;
    outputSensor << acq;
    outputSensor << acq;

    hub::InputSensor inputSensor { hub::io::Memory<decltype( buff )>( buff ) };

    hub::Acquisition acq2;
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
