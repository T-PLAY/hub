#include <catch2/catch_test_macros.hpp>

#include "test_common.hpp"

#include <InputSensor.hpp>
#include <OutputSensor.hpp>

// #include <server/Server.hpp>


TEST_CASE( "Server test : direct stream" ) {

    //    const std::string ipv4 = "127.0.0.1";
    //    const int port         = GET_RANDOM_PORT;
    constexpr int nOutput = 2;
    constexpr int nInput  = 2;

    std::vector<hub::Acquisition> acqs;
    constexpr int nAcqs    = 20;
    constexpr int dataSize = 9;
    for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
        unsigned char data[dataSize];
        for ( int i = 0; i < dataSize; ++i ) {
            data[i] = iAcq + i;
        }
        hub::Acquisition acq( iAcq + 1, iAcq + 2 );
        acq << hub::Measure { reinterpret_cast<const unsigned char*>( data ),
                                    dataSize,
                                    { { 3 }, hub::Format::BGR8 } };
        acqs.push_back( std::move( acq ) );
    }

    //    hub::Server server( port );
    //    server.setMaxClients( nOutput + nOutput * nInput );
    //    server.asyncRun();
    std::cout << "[Test] server end ------------------------------" << std::endl;

    for ( int iOutput = 0; iOutput < nOutput; ++iOutput ) {
        { // outputSensor

            hub::OutputSensor outputSensor(
                hub::SensorSpec { "sensorName", { { { 3 }, hub::Format::BGR8 } } },
                OutputStream( FILE_NAME )
                //                "stream",
                //                hub::net::ClientSocket( ipv4, port ) );
                //                ipv4, port
            );

            const auto& outputSensorSpec = outputSensor.getSpec();
            CHECK( outputSensorSpec.getAcquisitionSize() == dataSize );
            CHECK( outputSensorSpec.getSensorName() == "sensorName" );
            CHECK( outputSensorSpec.getResolutions().size() == 1 );
            CHECK( outputSensorSpec.getResolutions()[0].first.size() == 1 );
            CHECK( outputSensorSpec.getResolutions()[0].first.at( 0 ) == 3 );
            CHECK( outputSensorSpec.getResolutions()[0].second == hub::Format::BGR8 );

            for ( int iInput = 0; iInput < nInput; ++iInput ) {
                {
                    hub::InputSensor inputSensor(
                        //                        hub::io::InputStream( "stream",
                        //                        hub::net::ClientSocket( ipv4, port ) ) );
                        //                        hub::input::InputStreamServer( "stream", ipv4,
                        //                        port ) );
                        hub::input::InputStream( FILE_NAME ) );

                    const auto& inputSensorSpec = inputSensor.getSpec();
                    CHECK( inputSensorSpec.getAcquisitionSize() == dataSize );
                    CHECK( inputSensorSpec.getSensorName() == "sensorName" );
                    CHECK( inputSensorSpec.getResolutions().size() == 1 );
                    CHECK( inputSensorSpec.getResolutions()[0].first.size() == 1 );
                    CHECK( inputSensorSpec.getResolutions()[0].first.at( 0 ) == 3 );
                    CHECK( inputSensorSpec.getResolutions()[0].second == hub::Format::BGR8 );

                    for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
                        outputSensor << acqs[iAcq];
                        hub::Acquisition acq;
                        inputSensor >> acq;
                        CHECK( acq == acqs[iAcq] );
                    }
                    std::cout << "[Test] inputStream end ---------------------------------"
                              << std::endl;
                }
            }

        } // end outputSensor

        std::cout << "[Test] outputStream end ---------------------------------" << std::endl;

    } // loop

    std::cout << "[Test] done" << std::endl;
}
