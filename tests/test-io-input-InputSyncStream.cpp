#include <catch2/catch_test_macros.hpp>

#include "test_common.hpp"

#include <set>

#include <io/input/InputSyncStream.hpp>
#include <io/output/OutputStream.hpp>
// #include <server/Server.hpp>

#include <InputSensor.hpp>
#include <OutputSensor.hpp>

TEST_CASE( "InputSyncStream test" ) {

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

    //////////////////////

    std::cout << "ref2_acqs" << std::endl;
    const hub::Resolution ref_resolution2( { { 1 }, hub::Format::DOF6 } );
    const hub::SensorSpec ref_sensorSpec2( "sensorName2", { ref_resolution2 } );
    std::vector<hub::Acquisition> ref_acqs2;
    const int ref_dataSize2 = hub::res::computeAcquisitionSize( ref_resolution2 );
    float data2[7];
    for ( int iAcq = 0; iAcq < ref_nAcqs2; ++iAcq ) {
        for ( int i = 0; i < 7; ++i ) {
            data2[i] = ref2_offset + iAcq + 1;
        }
        ref_acqs2.emplace_back( ref2_offset + iAcq + 1, ref2_offset + iAcq + 1 );
        ref_acqs2.back() << hub::data::Measure(
            reinterpret_cast<const unsigned char*>( data2 ), ref_dataSize2, ref_resolution2 );

        std::cout << ref_acqs2.back() << std::endl;
    }
    std::cout << std::endl;

    //////////////////////

    std::cout << "ref_sync_acqs" << std::endl;
    std::vector<hub::Acquisition> ref_sync_acqs = computeSyncAcqs( ref_acqs, ref_acqs2 );

    std::cout << "sorted acqs" << std::endl;
    std::set<hub::Acquisition> sortedAcqs;
    for ( int i = 1; i < ref_acqs.size(); ++i ) {
        const auto& acq = ref_acqs.at( i );
        sortedAcqs.insert( acq.clone() );
    }
    for ( int i = 1; i < ref_acqs2.size(); ++i ) {
        const auto& acq = ref_acqs2.at( i );
        sortedAcqs.insert( acq.clone() );
    }
    assert( sortedAcqs.size() == ref_acqs.size() + ref_acqs2.size() - 2 );
    //            sortedAcqs.insert(ref_acqs.begin() + 1, ref_acqs.end());
    //            sortedAcqs.insert(ref_acqs2.begin() + 1, ref_acqs2.end());

    for ( const auto& acq : sortedAcqs ) {
        std::cout << acq << std::endl;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////

    {
        //        const std::string ipv4 = "127.0.0.1";
        //        const int port         = GET_RANDOM_PORT;

        //        hub::Server server( port );
        //        server.setMaxClients( 4 );
        //        server.asyncRun();

        {
            hub::OutputSensor outputSensor(
                //                ref_sensorSpec, "streamName", hub::net::ClientSocket( ipv4, port )
                //                ); ref_sensorSpec, "streamName", ipv4, port );
                ref_sensorSpec,
                OutputStream( "streamName" ) );
            std::cout << "outputSensor created" << std::endl;

            hub::OutputSensor outputSensor2(
                //                ref_sensorSpec2, "streamName2", hub::net::ClientSocket( ipv4, port
                //                ) ); ref_sensorSpec2, "streamName2", ipv4, port );
                ref_sensorSpec2,
                OutputStream( "streamName2" ) );
            std::cout << "outputSensor2 created" << std::endl;
            //            std::cout << "outputSensors created" << std::endl;

            //            hub::input::InputSyncStream<hub::input::InputStreamServer> inputStream(
            //            "streamName", "streamName2", ipv4, port );
            //            hub::input::InputSyncStream<hub::input::InputStream> inputStream(
            //            "streamName", "streamName2" );

            InputStream inputStream( "streamName" );
            hub::io::Input& input = inputStream;
            hub::SensorSpec sensorSpec;
            inputStream.read( sensorSpec );
            std::cout << "inputStream created : " << &inputStream << std::endl;

            InputStream inputStream2( "streamName2" );
            hub::io::Input& input2 = inputStream2;
            hub::SensorSpec sensorSpec2;
            inputStream2.read( sensorSpec2 );
            std::cout << "inputStream2 created : " << &inputStream2 << std::endl;

            CHECK( ref_sensorSpec == sensorSpec );
            CHECK( ref_sensorSpec2 == sensorSpec2 );

//            hub::input::InputSyncStream inputSyncStream( "streamName", "streamName2"
//                        );
//                        std::cout << "inputSyncStream created" << std::endl;

//                        hub::InputSensor inputSensor("streamName", "streamName2");
//                        hub::InputSensor inputSensor( std::move( inputSyncStream ) );
            //            hub::InputSensor inputSensor( InputSyncStream("streamName",
            //            "streamName2"));
            //            std::cout << "inputSensor created" << std::endl;

            //            CHECK( inputSensor.getSpec() == ref_sensorSpec + ref_sensorSpec2 );

            std::cout << "synching acqs" << std::endl;
            std::vector<hub::Acquisition> sync_acqs;
            std::thread thread = std::thread( [&]() {
                hub::Acquisition acq;
                for ( int i = 0; i < ref_sync_acqs.size(); ++i ) {

//                                        inputSensor >> acq;
                    //                    auto acq2 = inputStream >> inputStream2;
                    input >> input2 >> acq;
                    //                    (inputStream >> inputStream2) >> acq;
                    std::cout << "\tread synched acq: " << acq << std::endl;
                    sync_acqs.push_back( std::move( acq ) );
                }
            } );

//            std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );

            const auto & acqFront2 = ref_acqs2.front();
            std::cout << "write acq2: " << acqFront2 << std::endl;
            outputSensor2 << acqFront2;

//            std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );

            const auto & acqFront = ref_acqs.front();
            std::cout << "write acq: " << acqFront << std::endl;
            outputSensor << acqFront;

//            std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );


            for ( const auto& acq : sortedAcqs ) {
                if ( acq.getMeasures().front().getResolution() == ref_resolution ) {
                    assert( acq.getMeasures().front().getResolution().second == hub::Format::Y8 );
                    std::cout << "write acq: " << acq << std::endl;
                    outputSensor << acq;
                }
                else {
                    assert( acq.getMeasures().front().getResolution().second == hub::Format::DOF6 );
                    std::cout << "write acq2: " << acq << std::endl;
                    outputSensor2 << acq;
                }

//                std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
            }
            //            for ( const auto& acq : ref_acqs ) {
            //                outputSensor << acq;
            //            }

            //            for ( const auto& acq2 : ref_acqs2 ) {
            //                outputSensor2 << acq2;
            //            }

            assert( thread.joinable() );
            thread.join();
            std::cout << "end synching acqs" << std::endl;
//            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

            assert( sync_acqs.size() == ref_sync_acqs.size() );
            for ( int i = 0; i < ref_sync_acqs.size(); ++i ) {
                const auto& sync_acq = sync_acqs.at( i );
                CHECK( sync_acq == ref_sync_acqs.at( i ) );
            }
        }
        std::cout << "end output streams" << std::endl;
//        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }
    std::cout << "end server" << std::endl;
//    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
}
