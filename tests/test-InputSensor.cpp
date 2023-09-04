#include <catch2/catch_test_macros.hpp>

#include "test_common.hpp"

#include <InputSensor.hpp>
#include <OutputSensor.hpp>

#include <io/Memory.hpp>

TEST_CASE( "InputSensor test" ) {

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
        ref_acqs.emplace_back( ref_offset + iAcq + 1, ref_offset + iAcq + 1 );
        ref_acqs.back() << hub::data::Measure(
            reinterpret_cast<const unsigned char*>( data ), ref_dataSize, ref_resolution );
        std::cout << ref_acqs.back() << std::endl;
    }
    delete[] data;

    std::cout << std::endl;

    //////////////////////

    std::cout << "ref2_acqs" << std::endl;
    const hub::Resolution ref_resolution2( { { 1 }, hub::Format::DOF6 } );
    const hub::SensorSpec ref_sensorSpec2( "sensorName2", { ref_resolution2 } );
    std::vector<hub::Acquisition> ref_acqs2;
    const int ref_dataSize2 = hub::res::computeAcquisitionSize( ref_resolution2 );
    unsigned char* data2    = new unsigned char[ref_dataSize2];
    float* data2Float       = (float*)data2;
    for ( int iAcq = 0; iAcq < ref_nAcqs2; ++iAcq ) {
        for ( int i = 0; i < ref_dataSize2 / 4; ++i ) {
            //            data2[i] = ref2_offset + iAcq + 1;
            data2Float[i] = ref2_offset + iAcq + 1;
        }
        ref_acqs2.emplace_back( ref2_offset + iAcq + 1, ref2_offset + iAcq + 2 );
        ref_acqs2.back() << hub::data::Measure(
            reinterpret_cast<const unsigned char*>( data2 ), ref_dataSize2, ref_resolution2 );
        std::cout << ref_acqs2.back() << std::endl;
    }
    delete[] data2;

    std::cout << std::endl;

    std::cout << "ref_sync_acqs" << std::endl;
    std::vector<hub::Acquisition> ref_sync_acqs = computeSyncAcqs( ref_acqs, ref_acqs2 );

    ///////////////////////////////// TESTING EACH INPUT IMPLEMENTS

    std::cout << "----------------------------------------- Memory ----------------------------------------"
                 "---------------------"
              << std::endl;

    {
        std::vector<char> buff;
        hub::output::OutputMemory<decltype( buff )> outputMemory( buff );
        hub::input::InputMemory<decltype( buff )> inputMemory( buff );
        std::vector<char> buff2;
        hub::output::OutputMemory<decltype( buff2 )> outputMemory2( buff2 );
        hub::input::InputMemory<decltype( buff2 )> inputMemory2( buff2 );

        std::vector<hub::Acquisition> sync_acqs = synchronize( std::move( outputMemory ),
                                                               ref_sensorSpec,
                                                               ref_acqs,
                                                               std::move( outputMemory2 ),
                                                               ref_sensorSpec2,
                                                               ref_acqs2,
                                                               std::move( inputMemory ),
                                                               std::move( inputMemory2 ) );

        assert( sync_acqs.size() == ref_sync_acqs.size() );
        for ( int i = 0; i < sync_acqs.size(); ++i ) {
            const auto& acq2 = sync_acqs.at( i );

            CHECK( acq2 == ref_sync_acqs.at( i ) );
        }
    }

    std::cout << "----------------------------------------- File ----------------------------------------"
                 "---------------------"
              << std::endl;

    {
        {
            auto outputFile  = hub::output::OutputFile( "filepath.txt" );
            auto outputFile2 = hub::output::OutputFile( "filepath2.txt" );

            hub::OutputSensor outputSensor( ref_sensorSpec, std::move( outputFile ) );
            hub::OutputSensor outputSensor2( ref_sensorSpec2, std::move( outputFile2 ) );

            for ( const auto& acq : ref_acqs ) {
                outputSensor << acq;
            }
            for ( const auto& acq2 : ref_acqs2 ) {
                outputSensor2 << acq2;
            }
        }

        auto inputFile  = hub::input::InputFile( "filepath.txt" );
        auto inputFile2 = hub::input::InputFile( "filepath2.txt" );
        hub::InputSensor inputSensor( std::move( inputFile ) );
        hub::InputSensor inputSensor2( std::move( inputFile2 ) );

        hub::Acquisition acq;
        std::cout << "sync acqs" << std::endl;
        std::vector<hub::Acquisition> sync_acqs;
        while ( !inputSensor.getInput().isEnd() && !inputSensor2.getInput().isEnd() ) {

            inputSensor >> inputSensor2 >> acq;
            std::cout << acq << std::endl;
            sync_acqs.push_back( std::move( acq ) );
        }

        std::cout << std::endl;

        assert( sync_acqs.size() == ref_sync_acqs.size() );
        for ( int i = 0; i < sync_acqs.size(); ++i ) {
            const auto& acq2 = sync_acqs.at( i );

            CHECK( acq2 == ref_sync_acqs.at( i ) );
        }
    }

    std::cout << "----------------------------------------- Stream ----------------------------------------"
                 "---------------------"
              << std::endl;

#ifndef HUB_BUILD_SERVER
    {
        auto outputStream  = hub::output::OutputStream( __FILE_NAME__ );
        auto outputStream2 = hub::output::OutputStream( __FILE_NAME__ "2" );
        auto inputStream   = hub::input::InputStream( __FILE_NAME__ );
        auto inputStream2  = hub::input::InputStream( __FILE_NAME__ "2" );

        std::vector<hub::Acquisition> sync_acqs = synchronize( std::move( outputStream ),
                                                               ref_sensorSpec,
                                                               ref_acqs,
                                                               std::move( outputStream2 ),
                                                               ref_sensorSpec2,
                                                               ref_acqs2,
                                                               std::move( inputStream ),
                                                               std::move( inputStream2 ), true );

        assert( sync_acqs.size() == ref_sync_acqs.size() );
        for ( int i = 0; i < sync_acqs.size(); ++i ) {
            const auto& acq2 = sync_acqs.at( i );

            CHECK( acq2 == ref_sync_acqs.at( i ) );
        }
    }
#endif

    ///////////////////////////////////////////////////////////////////////////////////////////
}
