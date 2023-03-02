#include <catch2/catch_test_macros.hpp>

#include <OutputSensor.hpp>
#include <SensorSpec.hpp>
#include <Server.hpp>

#include <Asker.hpp>

TEST_CASE( "Asker test" ) {

    const hub::Resolution resolution( { 1 }, hub::Format::BGR8 );
    const hub::SensorSpec sensorSpec( "hello", { resolution } );
    const std::string streamName = "streamName";
    const std::string ipv4       = "127.0.0.1";
    const int port               = 12004;

    Server server( port );
    server.setMaxClients( 2 );
    server.asyncRun();
    //    server.ad

    {
        hub::OutputSensor outputSensor(
            sensorSpec, hub::io::OutputStream( streamName, hub::net::ClientSocket( ipv4, port ) ) );

        unsigned char data[3] = { 0, 1, 2 };
        const hub::Acquisition acq =
            std::move( hub::Acquisition( 0, 1 ) << hub::Measure( data, 3, resolution ) );
        outputSensor << acq;

        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

        {
            hub::Asker asker( ipv4, port );
            //            std::list<std::pair<std::string, hub::SensorSpec>> listStreams =
            //            asker.listStreams();
            auto listStreams = asker.listStreams();

            std::cout << "nb stream : " << listStreams.size() << std::endl;
            for ( const auto& [streamName2, sensorSpec2] : listStreams ) {
                std::cout << streamName2 << std::endl;
                CHECK( streamName == streamName2 );

                //        const auto & sensorSpec2 = asker.getSensorSpec(streamName2);
                std::cout << sensorSpec2 << std::endl;
                CHECK( sensorSpec == sensorSpec2 );

                auto acq2 = asker.getAcquisition( streamName2 );
                CHECK( acq == acq2 );
                std::cout << acq2 << std::endl;

                std::cout << "--------------------------------" << std::endl;
            }

            try {
                asker.getAcquisition( "fakeStream" );
                CHECK( false );
            }
            catch ( std::exception& ex ) {
                CHECK(true);
            }

        } // end asker
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    } // end outputSensor
    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    //////////////////////////////////////////////////////////////
}
