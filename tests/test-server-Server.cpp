#include <catch2/catch_test_macros.hpp>

#include "test-common.hpp"

#include <server/Server.hpp>
#include <server/StreamViewerClient.hpp>

TEST_CASE( "Server test" ) {

    const std::string ipv4 = "127.0.0.1";
    const int port         = GET_RANDOM_PORT;

    hub::Server server( port );
    server.setMaxClients( 2 );
    server.asyncRun();

    const int ref_offset    = 5;
    constexpr int ref_nAcqs = 10;

    std::cout << "ref_acqs" << std::endl;
    const hub::Resolution ref_resolution( { { 1 }, hub::Format::Y8 } );
    const hub::SensorSpec ref_sensorSpec( "sensorName", { ref_resolution } );
    std::vector<hub::Acquisition> ref_acqs;
    const int ref_dataSize = hub::res::computeAcquisitionSize( ref_resolution );
    unsigned char* data    = new unsigned char[ref_dataSize];
    for ( int iAcq = 0; iAcq < ref_nAcqs; ++iAcq ) {
        for ( int i = 0; i < ref_dataSize; ++i ) {
            data[i] = ref_offset + iAcq + 1;
        }
        ref_acqs.emplace_back( ref_offset + iAcq + 1, ref_offset + iAcq + 2 );
        ref_acqs.back() << hub::data::Measure(
            reinterpret_cast<const unsigned char*>( data ), ref_dataSize, ref_resolution );
        std::cout << ref_acqs.back() << std::endl;
    }
    delete[] data;

    std::cout << std::endl;

    //////////////////////

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////

    {
        hub::OutputSensor outputSensor(
//            ref_sensorSpec, "streamName", hub::net::ClientSocket( ipv4, port ) );
            ref_sensorSpec, "streamName",  ipv4, port );
        std::cout << "outputSensor created" << std::endl;

//        hub::io::InputStream inputStream( "streamName", hub::net::ClientSocket( ipv4, port ) );
        hub::input::InputStreamServer inputStream( "streamName", ipv4, port );
        hub::InputSensor inputSensor( std::move( inputStream ) );
        std::cout << "inputSensor created" << std::endl;

        for ( const auto& acq : ref_acqs ) {
            outputSensor << acq;
        }

        hub::Acquisition acq;
        std::cout << "sync acqs" << std::endl;
        std::vector<hub::Acquisition> acqs;
        for ( int i = 0; i < ref_acqs.size(); ++i ) {

            inputSensor >> acq;
            std::cout << acq << std::endl;
            acqs.push_back( std::move( acq ) );
        }

        assert( acqs.size() == ref_acqs.size() );
        for ( int i = 0; i < acqs.size(); ++i ) {
            const auto& acq2 = acqs.at( i );
            CHECK( acq2 == ref_acqs.at( i ) );
        }

        std::cout << "end test" << std::endl;
    }
}
