#include <catch2/catch_test_macros.hpp>

#include "test-common.hpp"

#include <io/InputSyncStream.hpp>
#include <io/OutputStream.hpp>
#include <server/Server.hpp>

TEST_CASE( "InputSyncStream test" ) {

    //    const int ref_offset    = 0;
    const int ref_offset    = 5;
    constexpr int ref_nAcqs = 10;

    //    const int ref2_offset    = 5;
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
    //    float* data2    = new float[ref_dataSize2];
    float data2[7];
    for ( int iAcq = 0; iAcq < ref_nAcqs2; ++iAcq ) {
        for ( int i = 0; i < 7; ++i ) {
            data2[i] = ref2_offset + iAcq + 1;
        }
        //        float value = ref2_offset + iAcq + 1;
        //        hub::data::Dof6 dof6()
        ref_acqs2.emplace_back( ref2_offset + iAcq + 1, ref2_offset + iAcq + 1 );
        ref_acqs2.back() << hub::data::Measure(
            reinterpret_cast<const unsigned char*>( data2 ), ref_dataSize2, ref_resolution2 );

        //        const auto & lastAcq = ref_acqs2.back();
        //        assert(lastAcq.getMeasures().size() == 1);
        //        const auto & measure = lastAcq.getMeasures().front();
        //        for ( int i = 0; i < ref_dataSize2; ++i ) {
        //            assert(measure.getData()[i] == ref2_offset + iAcq + 1);
        //        }
        std::cout << ref_acqs2.back() << std::endl;
    }
    //    delete[] data2;
    std::cout << std::endl;

    //////////////////////

    std::cout << "ref_sync_acqs" << std::endl;
    std::vector<hub::Acquisition> ref_sync_acqs;
    std::vector<int> min_dists( ref_acqs2.size(), 999999 );
    std::vector<int> iMin_dists( ref_acqs2.size(), -1 );

    int iAcq = 0;
    for ( const auto& acq : ref_acqs ) {
        int iMinDist = 0;
        int minDist  = computeDist( acq, ref_acqs2.front() );
        for ( int iAcq2 = 1; iAcq2 < ref_acqs2.size(); ++iAcq2 ) {
            const auto& acq2 = ref_acqs2.at( iAcq2 );
            const auto dist  = computeDist( acq, acq2 );
            if ( dist <= minDist ) {
                minDist  = dist;
                iMinDist = iAcq2;
            }
        }

        if ( minDist < min_dists.at( iMinDist ) ) {
            min_dists[iMinDist]  = minDist;
            iMin_dists[iMinDist] = iAcq;
        }
        ++iAcq;
    }

    for ( int i = 0; i < ref_acqs2.size(); ++i ) {
        if ( iMin_dists[i] != -1 ) {
            const auto& acq  = ref_acqs.at( iMin_dists.at( i ) );
            const auto& acq2 = ref_acqs2.at( i );
            ref_sync_acqs.push_back( acq.clone() );

            auto& syncAcq = ref_sync_acqs.back();
            syncAcq << acq2.getMeasures();

            std::cout << ref_sync_acqs.back() << std::endl;

            CHECK( syncAcq.getStart() == acq.getStart() );
            CHECK( syncAcq.getEnd() == acq.getEnd() );
            CHECK( syncAcq.getMeasures().size() == 2 );

            CHECK( acq.getMeasures().size() == 1 );
            const auto& measure = acq.getMeasures().front();
            CHECK( syncAcq.getMeasures().at( 0 ) == measure );

            CHECK( acq2.getMeasures().size() == 1 );
            const auto& measure2 = acq2.getMeasures().front();
            CHECK( syncAcq.getMeasures().at( 1 ) == measure2 );
        }
    }
    std::cout << std::endl;

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////

    {
        const std::string ipv4 = "127.0.0.1";
        const int port         = GET_RANDOM_PORT;

        hub::server::Server server( port );
        server.setMaxClients( 4 );
        server.asyncRun();

        {
            hub::io::OutputStream outputStream( "streamName",
                                                hub::net::ClientSocket( ipv4, port ) );
            hub::OutputSensor outputSensor( ref_sensorSpec, std::move( outputStream ) );

            hub::io::OutputStream outputStream2( "streamName2",
                                                 hub::net::ClientSocket( ipv4, port ) );
            hub::OutputSensor outputSensor2( ref_sensorSpec2, std::move( outputStream2 ) );
            std::cout << "outputSensors created" << std::endl;

            hub::io::InputSyncStream inputStream( "streamName", "streamName2", ipv4, port );
            hub::InputSensor inputSensor( std::move( inputStream ) );
            std::cout << "inputSensor created" << std::endl;
            CHECK( inputSensor.getSpec() == ref_sensorSpec + ref_sensorSpec2 );
            //            const hub::io::Input& input = inputSensor.getInput();

            for ( const auto& acq : ref_acqs ) {
                //            std::cout << "write: " << acq << std::endl;
                outputSensor << acq;
            }

            for ( const auto& acq2 : ref_acqs2 ) {
                //            std::cout << "write: " << acq << std::endl;
                outputSensor2 << acq2;
            }

            hub::Acquisition acq;
            std::cout << "sync acqs" << std::endl;
            std::vector<hub::Acquisition> sync_acqs;
            //        while ( !input.isEnd()  ) {
            for ( int i = 0; i < ref_sync_acqs.size(); ++i ) {

                inputSensor >> acq;
                std::cout << acq << std::endl;
                sync_acqs.push_back( std::move( acq ) );
            }

            assert( sync_acqs.size() == ref_sync_acqs.size() );
            for ( int i = 0; i < ref_sync_acqs.size(); ++i ) {
                const auto& sync_acq = sync_acqs.at( i );
                CHECK( sync_acq == ref_sync_acqs.at( i ) );
            }
        }
        std::cout << "end output streams" << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }
    std::cout << "end server" << std::endl;
    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
}
