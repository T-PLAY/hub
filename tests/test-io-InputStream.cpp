#include <catch2/catch_test_macros.hpp>

#include "test-common.hpp"

#include <io/InputStream.hpp>
#include <io/OutputStream.hpp>
#include <server/Server.hpp>

TEST_CASE( "InputStream test" ) {

    //    const int ref_offset    = 0;
    const int ref_offset    = 5;
    constexpr int ref_nAcqs = 10;

    //    const int ref2_offset    = 5;
//    const int ref2_offset    = 0;
//    constexpr int ref_nAcqs2 = 10;

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

    //    {
    const std::string ipv4 = "127.0.0.1";
    const int port         = GET_RANDOM_PORT;
    hub::server::Server server( port );
    server.setMaxClients( 2 );
    server.asyncRun();

    hub::io::OutputStream outputStream( "streamName", hub::net::ClientSocket( ipv4, port ) );
    hub::OutputSensor outputSensor( ref_sensorSpec, std::move( outputStream ) );
    std::cout << "outputSensor created" << std::endl;

    hub::io::InputStream inputStream( "streamName", hub::net::ClientSocket( ipv4, port ) );
    hub::InputSensor inputSensor( std::move( inputStream ) );
    CHECK(inputSensor.getSpec() == ref_sensorSpec);
    std::cout << "inputSensor created" << std::endl;
    //        hub::io::Input& input = inputSensor.getInput();

    for ( const auto& acq : ref_acqs ) {
        assert(! acq.isEmpty());
        outputSensor << acq;
    }

//        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    hub::Acquisition acq;
    std::cout << "acqs" << std::endl;
    std::vector<hub::Acquisition> acqs;
    //    while ( !input.isEnd()  ) {
    for ( int i = 0; i < ref_acqs.size(); ++i ) {

        inputSensor >> acq;
        assert(! acq.isEmpty());
        std::cout << acq << std::endl;
        acqs.push_back( std::move( acq ) );
    }

    assert( acqs.size() == ref_acqs.size() );
    for ( int i = 0; i < acqs.size(); ++i ) {
        const auto& acq2 = acqs.at( i );
        CHECK( acq2 == ref_acqs.at( i ) );
    }

    std::cout << "end test" << std::endl;
    //    }
}
