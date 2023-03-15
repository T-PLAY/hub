
#include "test-common.hpp"
#include <catch2/catch_test_macros.hpp>

#include <InputSensor.hpp>
#include <OutputSensor.hpp>

#include <Server.hpp>

// #include <io/Interface.hpp>

TEST_CASE( "InputSensor test" ) {

    hub::io::CyclicBuff buff;

    const auto resolution = hub::Resolution { { 1 }, hub::Format::BGR8 };
    hub::SensorSpec sensorSpec( "sensorName", { resolution } );
    unsigned char data[3] { 1, 2, 3 };
    hub::OutputSensor outputSensor( sensorSpec, hub::io::Ram( buff ) );

    auto acq = std::move( hub::Acquisition { 0, 1 } << hub::data::Measure { data, 3, resolution } );
    outputSensor << acq;
    outputSensor << acq;
    outputSensor << acq;

    hub::InputSensor inputSensor { hub::io::Ram( buff ) };

    auto acq2 = inputSensor.getAcquisition();
    CHECK( acq == acq2 );

    const auto& input = inputSensor.getInput();
    CHECK( !input.isEnd() );

    auto acqs = inputSensor.getAllAcquisitions();
    CHECK( acqs.size() == 2 );
    CHECK( acqs[0] == acq );
    CHECK( acqs[1] == acq );

    CHECK( input.isEnd() );

    const hub::Resolution resolution2( { 1 }, hub::Format::BGR8 );
    const hub::SensorSpec sensorSpec2( "hello", { resolution2 } );
    const std::string streamName = "streamName";
    const std::string ipv4       = "127.0.0.1";
    const int port               = getRandomPort();

    Server server( port );
    server.setMaxClients( 2 );
    server.asyncRun();

    hub::OutputSensor* outputSensor2 = new hub::OutputSensor(
        sensorSpec2, hub::io::OutputStream( streamName, hub::net::ClientSocket( ipv4, port ) ) );

    unsigned char data2[3] = { 0, 1, 2 };
    const hub::Acquisition acq3 =
        std::move( hub::Acquisition( 0, 1 ) << hub::data::Measure( data2, 3, resolution2 ) );
    *outputSensor2 << acq3;

    hub::InputSensor inputSensor2(
        hub::io::InputStream( streamName, "", hub::net::ClientSocket( ipv4, port ) ) );

    auto acq4 = inputSensor2.getAcquisition();
    CHECK( acq3 == acq4 );

    delete outputSensor2;

    try {

        auto acqs2 = inputSensor2.getAllAcquisitions();
        CHECK( false );
    }
    catch ( std::exception& ex ) {
        std::cout << "[test] catch execption : " << ex.what() << std::endl;
        CHECK( true );
    }
}
