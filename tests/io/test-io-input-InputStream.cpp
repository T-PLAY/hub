#include <catch2/catch_test_macros.hpp>

// #include "test_common.hpp"

#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>
// #include <server/Server.hpp>
// #include <InputSensor.hpp>
// #include <OutputSensor.hpp>

TEST_CASE( "InputStream test" ) {

    const int ref_offset    = 5;
    constexpr int ref_nAcqs = 10;

    std::cout << "[test] ref_acqs" << std::endl;
    const hub::sensor::Resolution ref_resolution( { { 1 }, hub::sensor::Format::Y8 } );
    const hub::sensor::SensorSpec ref_sensorSpec( "sensorName", { ref_resolution } );
    std::vector<hub::sensor::Acquisition> ref_acqs;
    const int ref_dataSize = hub::sensor::resolution::computeAcquisitionSize( ref_resolution );
    unsigned char* data    = new unsigned char[ref_dataSize];
    for ( int iAcq = 0; iAcq < ref_nAcqs; ++iAcq ) {
        for ( int i = 0; i < ref_dataSize; ++i ) {
            data[i] = ref_offset + iAcq + 1;
        }
        ref_acqs.emplace_back( ref_offset + iAcq + 1, ref_offset + iAcq + 2 );
        ref_acqs.back() << hub::Measure(
            reinterpret_cast<const unsigned char*>( data ), ref_dataSize, ref_resolution );
        std::cout << ref_acqs.back() << std::endl;
    }
    delete[] data;

    std::cout << std::endl;

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //    const std::string ipv4 = "127.0.0.1";
    //    const int port         = GET_RANDOM_PORT;
    //    hub::Server server( port );
    //    server.setMaxClients( 2 );
    //    server.asyncRun();

#ifndef HUB_BUILD_SERVER // not work because server is not allow to use stream without sensor
    {
        //        hub::sensor::OutputSensor outputSensor(
        //        hub::sensor::OutputSensor outputSensor(
        //            ref_sensorSpec,
        //            hub::output::OutputStream( FILE_NAME ) );
        hub::output::OutputStream outputStream( FILE_NAME );
        std::cout << "[test] outputSensor created" << std::endl;

        {
            try {
                hub::input::InputStream inputStream( "noStreamName" );
                assert( false );
            }
            catch ( std::exception& ex ) {
                std::cout << "[test] catch exception : '" << ex.what() << "'" << std::endl;

                CHECK( true );
            }
        }

        {
            hub::input::InputStream inputStream( FILE_NAME );
            //            hub::sensor::InputSensor inputSensor( std::move( inputStream ) );
            //            CHECK( inputSensor.getSpec() == ref_sensorSpec );
            std::cout << "[test] inputSensor created" << std::endl;

            std::cout << "[test] acqs" << std::endl;
            hub::sensor::Acquisition inputAcq;
            //            std::vector<hub::sensor::Acquisition> acqs;
            for ( int i = 0; i < ref_acqs.size(); ++i ) {
                //            for ( const auto& acq : ref_acqs ) {
                const auto& outputAcq = ref_acqs.at( i );
                assert( !outputAcq.isEmpty() );
                //                outputSensor << outputAcq;
                outputStream.write( outputAcq );
                //            }

                //            for ( int i = 0; i < ref_acqs.size(); ++i ) {

                //                inputSensor >> inputAcq;
                inputStream.read( inputAcq );
                assert( !inputAcq.isEmpty() );
                std::cout << inputAcq << std::endl;
                CHECK( outputAcq == inputAcq );
                //                acqs.push_back( std::move( inputAcq ) );
                //            }
            }

            //            assert( acqs.size() == ref_acqs.size() );
            //            for ( int i = 0; i < acqs.size(); ++i ) {
            //                const auto& acq2 = acqs.at( i );
            //                CHECK( acq2 == ref_acqs.at( i ) );
            //            }
            std::cout << "[test] end inputStream" << std::endl;
        }
        std::cout << "[test] end outputStream" << std::endl;
        //        std::cout << "[test] end server" << std::endl;
    }
#endif
}
