
#include <InputSensor.hpp>

#include <filesystem>
#include <thread>
// #include <cassert>

#include "../tests/test-common.hpp"
#include <InputSensor.hpp>
#include <server/Server.hpp>

/// \file
/// \brief  Main function
/// describes how use a file to init input/output sensor stream
///
int main() {
    /// Comments I would like to be documented in as well

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
    unsigned char* data2    = new unsigned char[ref_dataSize2];
    for ( int iAcq = 0; iAcq < ref_nAcqs2; ++iAcq ) {
        for ( int i = 0; i < ref_dataSize2; ++i ) {
            data2[i] = ref2_offset + iAcq + 1;
        }
        ref_acqs2.emplace_back( ref2_offset + iAcq + 1, ref2_offset + iAcq + 2 );
        ref_acqs2.back() << hub::data::Measure(
            reinterpret_cast<const unsigned char*>( data2 ), ref_dataSize2, ref_resolution2 );
        std::cout << ref_acqs2.back() << std::endl;
    }
    delete[] data2;
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

            assert( syncAcq.getStart() == acq.getStart() );
            assert( syncAcq.getEnd() == acq.getEnd() );
            assert( syncAcq.getMeasures().size() == 2 );

            assert( acq.getMeasures().size() == 1 );
            const auto& measure = acq.getMeasures().front();
            assert( syncAcq.getMeasures().at( 0 ) == measure );

            assert( acq2.getMeasures().size() == 1 );
            const auto& measure2 = acq2.getMeasures().front();
            assert( syncAcq.getMeasures().at( 1 ) == measure2 );
        }
    }
    std::cout << std::endl;

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const std::string ipv4 = "127.0.0.1";
    const int port         = GET_RANDOM_PORT;

    hub::server::Server server( port );
    server.setMaxClients( 2 );
    server.asyncRun();

//    {
        hub::io::OutputStream outputStream( "streamName", hub::net::ClientSocket( ipv4, port ) );
        hub::OutputSensor outputSensor( ref_sensorSpec, std::move( outputStream ) );
        std::cout << "outputSensor created" << std::endl;

        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

//        {
            hub::io::InputStream inputStream(
                "streamName", hub::net::ClientSocket( ipv4, port ) );
            hub::InputSensor inputSensor( std::move( inputStream ) );
            std::cout << "inputSensor created" << std::endl;
//            hub::io::Input& input = inputSensor.getInput();

//            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

            //    hub::io::OutputStream outputStream2("streamName2", hub::net::ClientSocket(ipv4,
            //    port)); hub::OutputSensor outputSensor2( ref_sensorSpec2, std::move( outputStream2
            //    )
            //    );

            //    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

            //    hub::io::InputStream inputStream2("streamName2", "");
            //    hub::InputSensor inputSensor2(std::move(inputStream2));
            //    hub::io::Input& input2 = inputSensor.getInput();

            //    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

            for ( const auto& acq : ref_acqs ) {
                //            std::cout << "write: " << acq << std::endl;
                outputSensor << acq;
            }
            //    for ( const auto& acq2 : ref_acqs2 ) {
            //        outputSensor2 << acq2;
            //    }

            hub::Acquisition acq;
            //        std::cout << "sync acqs" << std::endl;
            std::vector<hub::Acquisition> acqs;
            //    while ( !input.isEnd()  ) {
            for ( int i = 0; i < ref_acqs.size(); ++i ) {

                inputSensor >> acq;
                //            std::cout << "acqs : " << acq << std::endl;
                acqs.push_back( std::move( acq ) );
            }

            assert( acqs.size() == ref_acqs.size() );
            for ( int i = 0; i < acqs.size(); ++i ) {
                const auto& acq2 = acqs.at( i );
                assert( acq2 == ref_acqs.at( i ) );
            }

            std::cout << "end test" << std::endl;
//        }
//        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
//    }
//    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    //    {
    //        hub::SensorSpec sensorSpec;
    //        // clang-format off
    //        {
    //            // startConstruction
    //// bin/server is running locally on port 4042
    //// the stream id name 'myStream' is streaming to the server
    // hub::InputSensor inputSensor(
    //     hub::io::InputStream( "myStream", "", hub::net::ClientSocket( "127.0.0.1", 4042 ) ) );
    //             // endConstruction

    //            // startFunctional
    // auto acq = inputSensor.getAcquisition();
    // auto acqs = inputSensor.getAllAcquisitions();
    //            // endFunctional
    //        }

    //        {
    //            // startConstruction2
    //// read saved acquisitions from file
    // hub::InputSensor inputSensor(
    //     hub::io::File( std::fstream("file.txt", std::ios::binary | std::ios::in ) ) );
    //             // endConstruction2
    //         }

    //        {
    //            // startConstruction3
    //// get acqusitions from different threads
    // hub::io::CyclicBuff buff;
    // hub::InputSensor inputSensor{ hub::io::Ram( buff ) };
    //             // endConstruction3
    //         }
    //         // clang-format on
    //     }

    return 0;
}
