
#include <sensor/InputSensor.hpp>

#include <filesystem>
#include <thread>

#include <sensor/InputSensor.hpp>
#include <server/Server.hpp>

/// \file
/// \brief  Main function
/// describes how use a file to init input/output sensor stream
///
int main() {
    /// Comments I would like to be documented in as well

    /// startConstruction
    /// outputSensor
    /// endConstruction

    /// startConstruction2
    /// outputSensor2
    /// endConstruction2

    /// startConstruction3
    /// outputSensor3
    /// endConstruction3

    /// startFunctional
    /// inputSensor
    /// endFunctional

    // const int ref_offset    = 5;
    // constexpr int ref_nAcqs = 10;

    // const int ref2_offset    = 0;
    // constexpr int ref_nAcqs2 = 10;

    // std::cout << "ref_acqs" << std::endl;
    // const hub::Resolution ref_resolution( { { 1 }, hub::format::Y8 } );
    // const hub::sensor::SensorSpec ref_sensorSpec( "sensorName", { ref_resolution } );
    // std::vector<hub::sensor::Acquisition> ref_acqs;
    // const int ref_dataSize = hub::sensor::resolution::computeAcquisitionSize( ref_resolution );
    // unsigned char* data    = new unsigned char[ref_dataSize];
    // for ( int iAcq = 0; iAcq < ref_nAcqs; ++iAcq ) {
    //     for ( int i = 0; i < ref_dataSize; ++i ) {
    //         data[i] = ref_offset + iAcq + 1;
    //     }
    //     ref_acqs.emplace_back( ref_offset + iAcq + 1, ref_offset + iAcq + 2 );
    //     ref_acqs.back() << hub::Measure(
    //         reinterpret_cast<const unsigned char*>( data ), ref_dataSize, ref_resolution );
    //     std::cout << ref_acqs.back() << std::endl;
    // }
    // delete[] data;

    // std::cout << std::endl;

    // //////////////////////

    // std::cout << "ref2_acqs" << std::endl;
    // const hub::Resolution ref_resolution2( { { 1 }, hub::format::Dof6 } );
    // const hub::sensor::SensorSpec ref_sensorSpec2( "sensorName2", { ref_resolution2 } );
    // std::vector<hub::sensor::Acquisition> ref_acqs2;
    // const int ref_dataSize2 = hub::sensor::resolution::computeAcquisitionSize( ref_resolution2 );
    // unsigned char* data2    = new unsigned char[ref_dataSize2];
    // for ( int iAcq = 0; iAcq < ref_nAcqs2; ++iAcq ) {
    //     for ( int i = 0; i < ref_dataSize2; ++i ) {
    //         data2[i] = ref2_offset + iAcq + 1;
    //     }
    //     ref_acqs2.emplace_back( ref2_offset + iAcq + 1, ref2_offset + iAcq + 2 );
    //     ref_acqs2.back() << hub::Measure(
    //         reinterpret_cast<const unsigned char*>( data2 ), ref_dataSize2, ref_resolution2 );
    //     std::cout << ref_acqs2.back() << std::endl;
    // }
    // delete[] data2;
    // std::cout << std::endl;

    // //////////////////////

    // std::cout << "ref_sync_acqs" << std::endl;
    // std::vector<hub::sensor::Acquisition> ref_sync_acqs;
    // std::vector<int> min_dists( ref_acqs2.size(), 999999 );
    // std::vector<int> iMin_dists( ref_acqs2.size(), -1 );

    // int iAcq = 0;
    // for ( const auto& acq : ref_acqs ) {
    //     int iMinDist = 0;
    //     int minDist  = computeDist( acq, ref_acqs2.front() );
    //     for ( int iAcq2 = 1; iAcq2 < ref_acqs2.size(); ++iAcq2 ) {
    //         const auto& acq2 = ref_acqs2.at( iAcq2 );
    //         const auto dist  = computeDist( acq, acq2 );
    //         if ( dist <= minDist ) {
    //             minDist  = dist;
    //             iMinDist = iAcq2;
    //         }
    //     }

    //     if ( minDist < min_dists.at( iMinDist ) ) {
    //         min_dists[iMinDist]  = minDist;
    //         iMin_dists[iMinDist] = iAcq;
    //     }
    //     ++iAcq;
    // }

    // for ( int i = 0; i < ref_acqs2.size(); ++i ) {
    //     if ( iMin_dists[i] != -1 ) {
    //         const auto& acq  = ref_acqs.at( iMin_dists.at( i ) );
    //         const auto& acq2 = ref_acqs2.at( i );
    //         ref_sync_acqs.push_back( acq.clone() );

    //         auto& syncAcq = ref_sync_acqs.back();
    //         syncAcq << acq2.getMeasures();

    //         std::cout << ref_sync_acqs.back() << std::endl;

    //         assert( syncAcq.getStart() == acq.getStart() );
    //         assert( syncAcq.getEnd() == acq.getEnd() );
    //         assert( syncAcq.getMeasures().size() == 2 );

    //         assert( acq.getMeasures().size() == 1 );
    //         const auto& measure = acq.getMeasures().front();
    //         assert( syncAcq.getMeasures().at( 0 ) == measure );

    //         assert( acq2.getMeasures().size() == 1 );
    //         const auto& measure2 = acq2.getMeasures().front();
    //         assert( syncAcq.getMeasures().at( 1 ) == measure2 );
    //     }
    // }
    // std::cout << std::endl;

    // //////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // const std::string ipv4 = "127.0.0.1";
    // const int port         = GET_RANDOM_PORT;

    // hub::server::Server server( port );
    // server.setMaxClients( 2 );
    // server.asyncRun();

    // hub::sensor::OutputSensor outputSensor(
    //     ref_sensorSpec, "streamName", hub::net::ClientSocket( ipv4, port ) );
    // std::cout << "outputSensor created" << std::endl;

    // std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    // hub::input::InputStream inputStream( "streamName", hub::net::ClientSocket( ipv4, port ) );
    // hub::sensor::InputSensor inputSensor( std::move( inputStream ) );
    // std::cout << "inputSensor created" << std::endl;

    // for ( const auto& acq : ref_acqs ) {
    //     outputSensor << acq;
    // }

    // hub::sensor::Acquisition acq;
    // std::vector<hub::sensor::Acquisition> acqs;
    // for ( int i = 0; i < ref_acqs.size(); ++i ) {

    //     inputSensor >> acq;
    //     acqs.push_back( std::move( acq ) );
    // }

    // assert( acqs.size() == ref_acqs.size() );
    // for ( int i = 0; i < acqs.size(); ++i ) {
    //     const auto& acq2 = acqs.at( i );
    //     assert( acq2 == ref_acqs.at( i ) );
    // }

    // std::cout << "end test" << std::endl;

    //// bin/server is running locally on port 4042
    //// the stream id name 'myStream' is streaming to the server

    //// read saved acquisitions from file

    //// get acqusitions from different threads

    return 0;
}
