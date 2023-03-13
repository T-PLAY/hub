
#include <catch2/catch_test_macros.hpp>
#include "test-common.hpp"

#include <InputSensor.hpp>
#include <OutputSensor.hpp>

#include <Server.hpp>

// #include <io/Interface.hpp>

TEST_CASE( "InputSensor test" ) {

    hub::io::CyclicBuff buff;
    //    hub::io::Memory<
    //    hub::io::Ram ram(buff);

    //    hub::InputSensor inputSensor(hub::io::Ram(buff));

    const auto resolution = hub::Resolution { { 1 }, hub::Format::BGR8 };
    hub::SensorSpec sensorSpec( "sensorName", { resolution } );
    unsigned char data[3] { 1, 2, 3 };
    hub::OutputSensor outputSensor( sensorSpec, hub::io::Ram( buff ) );
    auto& output = outputSensor.getOutput();

    auto acq = std::move( hub::Acquisition { 0, 1 } << hub::data::Measure { data, 3, resolution } );
    //    acq << hub::Measure{data, 3, resolution};
    outputSensor << acq;
    outputSensor << acq;
    outputSensor << acq;

    hub::InputSensor inputSensor { hub::io::Ram( buff ) };

    auto acq2 = inputSensor.getAcquisition();
    CHECK( acq == acq2 );

    //    CHECK(inputSensor.)
    auto& input = inputSensor.getInput();
    CHECK( !input.isEnd() );

    //    output.close();
    auto acqs = inputSensor.getAllAcquisitions();
    CHECK( acqs.size() == 2 );
    CHECK( acqs[0] == acq );
    CHECK( acqs[1] == acq );

    CHECK( input.isEnd() );

    {
        const hub::Resolution resolution( { 1 }, hub::Format::BGR8 );
        const hub::SensorSpec sensorSpec( "hello", { resolution } );
        const std::string streamName = "streamName";
        const std::string ipv4       = "127.0.0.1";
//        srand( (unsigned)time( NULL ) );
//        const int port = rand() % 65535;
    const int port = getRandomPort();

        Server server( port );
        server.setMaxClients( 2 );
        server.asyncRun();

        hub::OutputSensor* outputSensor = new hub::OutputSensor(
            sensorSpec, hub::io::OutputStream( streamName, hub::net::ClientSocket( ipv4, port ) ) );

        unsigned char data[3] = { 0, 1, 2 };
        const hub::Acquisition acq =
            std::move( hub::Acquisition( 0, 1 ) << hub::data::Measure( data, 3, resolution ) );
        *outputSensor << acq;

        hub::InputSensor inputSensor(
            hub::io::InputStream( streamName, "", hub::net::ClientSocket( ipv4, port ) ) );

        //        output.close();

        auto acq2 = inputSensor.getAcquisition();
        CHECK( acq == acq2 );

        delete outputSensor;

        try {

            auto acqs = inputSensor.getAllAcquisitions();
            CHECK( false );
        }
        catch ( std::exception& ex ) {
            CHECK( true );
        }
    }
}
