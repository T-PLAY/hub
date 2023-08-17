#include <catch2/catch_test_macros.hpp>

#include "test-common.hpp"

#include <io/InputStream.hpp>
#include <io/OutputStream.hpp>
#include <server/Server.hpp>

TEST_CASE( "OutputStream test" ) {

    const int ref_offset    = 5;
    constexpr int ref_nAcqs = 10;

    const int ref2_offset    = 0;
    constexpr int ref_nAcqs2 = 10;

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

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const std::string ipv4 = "127.0.0.1";
    const int port         = GET_RANDOM_PORT;
    hub::server::Server server( port );
    server.setMaxClients( 2 );
    server.asyncRun();

    hub::OutputSensor outputSensor(
//        ref_sensorSpec, "streamName", hub::net::ClientSocket( ipv4, port ) );
        ref_sensorSpec, "streamName",ipv4, port );
    std::cout << "outputSensor created" << std::endl;

    {
        try {
            // unable to open same stream name twice
            hub::io::OutputStream outputStream2( "streamName",
//                                                 hub::net::ClientSocket( ipv4, port ) );
                                                 ipv4, port );
            CHECK( false );
            hub::OutputSensor outputSensor2(
//                ref_sensorSpec, "streamName", hub::net::ClientSocket( ipv4, port ) );
                ref_sensorSpec, "streamName", ipv4, port );
            std::cout << "outputSensor2 created" << std::endl;
        }
        catch ( std::exception& ex ) {
            std::cout << "[test] catch exception : " << ex.what() << std::endl;
        }
    }
}
