#include <catch2/catch_test_macros.hpp>
#include "test-common.hpp"

#include <cstring>
#include <iostream>
#include <string>

#include <io/Input.hpp>

#include <io/Memory.hpp>

TEST_CASE( "Input test" ) {

    {
        std::string ref_string    = "hello";
        const auto ref_resolution = hub::Resolution { { 1 }, hub::Format::BGR8 };
        hub::SensorSpec ref_sensorSpec( "sensorName", { ref_resolution } );
        unsigned char data[3] { 1, 2, 3 };
        hub::Acquisition ref_acq = std::move( hub::Acquisition { 0, 1 }
                                              << hub::data::Measure { data, 3, ref_resolution } );
        const char ref_str[]     = "hello";
        hub::Any ref_any_int( 5 );
        hub::Any ref_any_double( 5.0 );
        hub::Any ref_any_string( std::string( "hello" ) );
        hub::Any ref_any_constCharPtr( "hello" );
        hub::Any ref_any_mat4( hub::data::Mat4( 0.0 ) );
        //    hub::Any ref_any_mesh(hub::data::Mesh);

        std::vector<char> buff;

        hub::io::Memory<decltype( buff )> memory( buff );

        memory.write( ref_string );
        memory.write( ref_sensorSpec );
        memory.write( ref_acq );
        memory.write( ref_str );

        memory.write( ref_any_int );
        memory.write( ref_any_double );
        memory.write( ref_any_string );
        memory.write( ref_any_constCharPtr );
        memory.write( ref_any_mat4 );

        std::string string;
        memory.read( string );
        CHECK( ref_string == string );

        hub::SensorSpec sensorSpec;
        memory.read( sensorSpec );
        CHECK( ref_sensorSpec == sensorSpec );

        hub::Acquisition acq;
        memory.read( acq );
        CHECK( ref_acq == acq );

        char str[80];
        memory.read( str );
        CHECK( str != ref_str );
        CHECK( !strcmp( str, ref_str ) );

        hub::Any any_int;
        memory.read( any_int );
        CHECK( ref_any_int == any_int );

        hub::Any any_double;
        memory.read( any_double );
        CHECK( ref_any_double == any_double );

        hub::Any any_string;
        memory.read( any_string );
        CHECK( ref_any_string == any_string );

        hub::Any any_constCharPtr;
        memory.read( any_constCharPtr );
        CHECK( ref_any_constCharPtr == any_constCharPtr );

        hub::Any any_mat4;
        memory.read( any_mat4 );
        std::cout << "ref_mat4: " << ref_any_mat4 << std::endl;
        std::cout << "mat4: " << any_mat4 << std::endl;
        assert( ref_any_mat4 == any_mat4 );
        CHECK( ref_any_mat4 == any_mat4 );

        CHECK( memory.isEnd() );
    }

    /////////////////////////////////////////////////////////////////////////////////////

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

    const std::string ipv4 = "127.0.0.1";
    const int port         = GET_RANDOM_PORT;

//    hub::server::Server server( port );
//    server.setMaxClients( 4 );
//    server.asyncRun();

    std::vector<char> buff;
    hub::io::Memory<decltype( buff )> memory( buff );
    hub::io::Output & output = memory;

    std::vector<char> buff2;
    hub::io::Memory<decltype( buff2 )> memory2( buff2 );
    hub::io::Output & output2 = memory2;


    hub::io::Input & input = memory;
    hub::io::Input & input2 = memory2;

//    for ( const auto& acq : ref_acqs ) {
//        output << acq;
//    }

//    for ( const auto& acq2 : ref_acqs2 ) {
//        output2 << acq2;
//    }

//    hub::Acquisition acq;
//    std::cout << "sync acqs" << std::endl;
//    std::vector<hub::Acquisition> sync_acqs;
//    //        while ( !input.isEnd()  ) {
//    for ( int i = 0; i < ref_sync_acqs.size(); ++i ) {

//        inputSensor >> acq;
//        std::cout << acq << std::endl;
//        sync_acqs.push_back( std::move( acq ) );
//    }

//    assert( sync_acqs.size() == ref_sync_acqs.size() );
//    for ( int i = 0; i < ref_sync_acqs.size(); ++i ) {
//        const auto& sync_acq = sync_acqs.at( i );
//        CHECK( sync_acq == ref_sync_acqs.at( i ) );
//    }

    std::cout << "end test" << std::endl;
}
