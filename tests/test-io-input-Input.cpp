#include "test_common.hpp"
#include <catch2/catch_test_macros.hpp>

#include <cstring>
#include <iostream>
#include <string>

#include <Input.hpp>

#include <io/Memory.hpp>

//#include <memory>

TEST_CASE( "Input test" ) {

    {
        std::string ref_string    = "hello";
        const auto ref_resolution = hub::Resolution { { 1 }, hub::Format::BGR8 };
        hub::SensorSpec::MetaData ref_metaData;
        ref_metaData["hello"] = 1;
        hub::SensorSpec ref_sensorSpec( "sensorName", { ref_resolution }, ref_metaData );
        unsigned char data[3] { 1, 2, 3 };
        hub::Acquisition ref_acq = std::move( hub::Acquisition { 0, 1 }
                                              << hub::Measure { data, 3, ref_resolution } );
        const char ref_str[]     = "hello";
        hub::Any ref_any_int( 5 );
        hub::Any ref_any_double( 5.0 );
        hub::Any ref_any_string( std::string( "hello" ) );
        hub::Any ref_any_constCharPtr( "hello" );
        hub::Any ref_any_mat4( hub::data::Mat4( 0.0 ) );
        const std::string meshPath = HUB_PROJECT_DIR "assets/";
        hub::data::Mesh ref_mesh( meshPath + "quad" );
        hub::Any ref_any_mesh( ref_mesh );

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
        memory.write( ref_any_mesh );

        std::string string;
        memory.read( string );
        CHECK( ref_string == string );

        hub::SensorSpec sensorSpec;
        memory.read( sensorSpec );
        CHECK( ref_sensorSpec == sensorSpec );
        CHECK( ref_metaData == sensorSpec.getMetaData() );

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

        hub::Any any_mesh;
        memory.read( any_mesh );
        assert( ref_any_mesh == any_mesh );

        CHECK( memory.isEnd() );
    }

    /////////////////////////////////////////////////////////////////////////////////////

    {
        std::vector<char> buff;
        hub::io::Memory<decltype( buff )> memory( buff );
        hub::Output& output = memory;

        hub::Input& input = memory;

        std::list<int> ints { 0, 1, 2 };
        output << ints;

        std::list<int> ints2;
        input >> ints2;

        for ( int i = 0; i < 3; ++i ) {
            assert( ints.front() == ints2.front() );
            ints.pop_front();
            ints2.pop_front();
        }
    }

    std::cout << "---------------------------------------------------------------------------------"
                 "-----------------------"
              << std::endl;

    using Acqs = std::vector<hub::Acquisition>;
    using Acqss = std::vector<Acqs>;
    Acqss ref_acqss( 2 );
    Acqss ref_acqss2( 2 );


    using Acqsss = std::pair<const Acqss &, const Acqss &>;

    const Acqsss ref_acqsss = {ref_acqss, ref_acqss2};
    const Acqsss ref_reverse_acqsss = {ref_acqss2, ref_acqss};

    const Acqsss ref_acqssss[2] = {ref_acqsss, ref_reverse_acqsss};


//    const Acqss & ref_acqsss[2][2] = {{ref_acqsss, ref_acqss2}, {ref_acqss2, ref_acqsss}};

//    const std:: & ref_acqsss = {ref_acqss, ref_acqss2};
//    std::vector<const std::vector<std::vector<hub::Acquisition>>*> ref_acqsss = {&ref_acqss};
//    const auto & refss = {{ref_acqss, ref_acqss2}, {ref_acqss2, ref_acqss}};


    {
        const int ref_offset    = 5;
        constexpr int ref_nAcqs = 10;

        const int ref2_offset    = 0;
        constexpr int ref_nAcqs2 = 10;

        const hub::Resolution ref_resolution( { { 1 }, hub::Format::Y8 } );
        std::vector<hub::Acquisition>& ref_acqs = ref_acqss.at( 0 );
        const int ref_dataSize = hub::res::computeAcquisitionSize( ref_resolution );
        unsigned char* data    = new unsigned char[ref_dataSize];
        for ( int iAcq = 0; iAcq < ref_nAcqs; ++iAcq ) {
            for ( int i = 0; i < ref_dataSize; ++i ) {
                data[i] = ref_offset + iAcq + 1;
            }
            ref_acqs.emplace_back( ref_offset + iAcq + 1, ref_offset + iAcq + 1 );
            ref_acqs.back() << hub::Measure(
                reinterpret_cast<const unsigned char*>( data ), ref_dataSize, ref_resolution );
        }
        delete[] data;

        const hub::Resolution ref_resolution2( { { 1 }, hub::Format::DOF6 } );
        std::vector<hub::Acquisition>& ref_acqs2 = ref_acqss2.at( 0 );
        const int ref_dataSize2 = hub::res::computeAcquisitionSize( ref_resolution2 );
        float data2[7];
        for ( int iAcq = 0; iAcq < ref_nAcqs2; ++iAcq ) {
            for ( int i = 0; i < 7; ++i ) {
                data2[i] = ref2_offset + iAcq + 1;
            }
            ref_acqs2.emplace_back( ref2_offset + iAcq + 1, ref2_offset + iAcq + 1 );
            ref_acqs2.back() << hub::Measure(
                reinterpret_cast<const unsigned char*>( data2 ), ref_dataSize2, ref_resolution2 );
        }
    }

    {
        const int ref_offset    = 2;
        constexpr int ref_nAcqs = 10;
        constexpr int ref_step  = 3;

        const int ref2_offset    = 5;
        constexpr int ref_nAcqs2 = 10;
        constexpr int ref_step2  = 1;

        const hub::Resolution ref_resolution( { { 1 }, hub::Format::Y8 } );
        std::vector<hub::Acquisition>& ref_acqs = ref_acqss.at( 1 );
        const int ref_dataSize = hub::res::computeAcquisitionSize( ref_resolution );
        unsigned char* data    = new unsigned char[ref_dataSize];
        for ( int iAcq = 0; iAcq < ref_nAcqs; ++iAcq ) {
            for ( int i = 0; i < ref_dataSize; ++i ) {
                data[i] = ref_offset + iAcq * ref_step + 1;
            }
            ref_acqs.emplace_back( ref_offset + iAcq * ref_step + 1,
                                   ref_offset + iAcq * ref_step + 1 );
            ref_acqs.back() << hub::Measure(
                reinterpret_cast<const unsigned char*>( data ), ref_dataSize, ref_resolution );
        }
        delete[] data;

        const hub::Resolution ref_resolution2( { { 1 }, hub::Format::DOF6 } );
        std::vector<hub::Acquisition>& ref_acqs2 = ref_acqss2.at( 1 );
        const int ref_dataSize2 = hub::res::computeAcquisitionSize( ref_resolution2 );
        float data2[7];
        for ( int iAcq = 0; iAcq < ref_nAcqs2; ++iAcq ) {
            for ( int i = 0; i < 7; ++i ) {
                data2[i] = ref2_offset + iAcq * ref_step2 + 1;
            }
            ref_acqs2.emplace_back( ref2_offset + iAcq * ref_step2 + 1,
                                    ref2_offset + iAcq * ref_step2 + 1 );
            ref_acqs2.back() << hub::Measure(
                reinterpret_cast<const unsigned char*>( data2 ), ref_dataSize2, ref_resolution2 );
        }
    }

    for ( int j = 0; j < 2; ++j ) {
        for ( int i = 0; i < ref_acqss.size(); ++i ) {

            const int idx = j * ref_acqss.size() + i;

//            const auto& ref_acqs = ref_acqss.at( i );
            const auto& ref_acqs = ref_acqssss[j].first.at(i);

            std::cout << "ref_acqs " << idx << std::endl;
            for ( const auto& acq : ref_acqs ) {
                std::cout << acq << std::endl;
            }
            std::cout << std::endl;

//            const auto& ref_acqs2 = ref_acqss2.at( i );
            const auto& ref_acqs2 = ref_acqssss[j].second.at(i);
            std::cout << "ref_acqs2 " << idx << std::endl;
            for ( const auto& acq : ref_acqs2 ) {
                std::cout << acq << std::endl;
            }
            std::cout << std::endl;

            std::cout << "ref_sync_acqs " << idx << std::endl;
            auto ref_sync_acqs = computeSyncAcqs( ref_acqs, ref_acqs2 );

            std::vector<char> buff;
            hub::io::Memory<decltype( buff )> memory( buff );
            hub::Output& output = memory;

            std::vector<char> buff2;
            hub::io::Memory<decltype( buff2 )> memory2( buff2 );
            hub::Output& output2 = memory2;

            hub::Input& input  = memory;
            hub::Input& input2 = memory2;

            for ( int i = 0; i < ref_acqs.size(); ++i ) {
                const auto& acq = ref_acqs.at( i );
                if ( i % 3 == 0 ) { output << acq; }
                else if ( i % 3 == 1 ) { output.write( acq ); }
                else { output.put( acq ); }
            }

            for ( const auto& acq2 : ref_acqs2 ) {
                output2 << acq2;
            }

            std::cout << "sync acqs " << idx << std::endl;
            std::vector<hub::Acquisition> sync_acqs;
            while ( !input.isEnd() && !input2.isEnd() ) {

                hub::Acquisition acq;
                input >> input2 >> acq;
                std::cout << acq << std::endl;
                sync_acqs.push_back( std::move( acq ) );
            }

            assert( sync_acqs.size() == ref_sync_acqs.size() );
            for ( int i = 0; i < ref_sync_acqs.size(); ++i ) {
                const auto& sync_acq = sync_acqs.at( i );
                CHECK( sync_acq == ref_sync_acqs.at( i ) );
            }
            std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"
                      << std::endl;
        }
    }

    std::cout << "end test" << std::endl;
}

////        std::vector<hub::Acquisition> ref_sync_acqs;
//        auto ref_sync_acqs = computeSyncAcqs(ref_acqs, ref_acqs2);

//        std::vector<char> buff;
//        hub::io::Memory<decltype( buff )> memory( buff );
//        hub::Output& output = memory;

//        std::vector<char> buff2;
//        hub::io::Memory<decltype( buff2 )> memory2( buff2 );
//        hub::Output& output2 = memory2;

//        hub::Input& input  = memory;
//        hub::Input& input2 = memory2;

//        for ( int i = 0; i < ref_acqs.size(); ++i ) {
//            const auto& acq = ref_acqs.at( i );
//            if ( i % 3 == 0 ) { output << acq; }
//            else if ( i % 3 == 1 ) {
//                output.write( acq );
//            }
//            else {
//                output.put( acq );
//            }
//        }

//        for ( const auto& acq2 : ref_acqs2 ) {
//            output2 << acq2;
//        }

//        std::cout << "sync acqs" << std::endl;
//        std::vector<hub::Acquisition> sync_acqs;
//        while ( !input.isEnd() && !input2.isEnd() ) {

//            hub::Acquisition acq;
//            input >> input2 >> acq;
//            std::cout << acq << std::endl;
//            sync_acqs.push_back( std::move( acq ) );
//        }

//        assert( input2.isEnd() );
//        assert( !input.isEnd() );

//        auto acq2 = input.get<hub::Acquisition>();
//        auto acqs = input.getAll<std::vector<hub::Acquisition>>();
//        CHECK( acqs.size() == ref_acqs.size() - ref_sync_acqs.size() - 1 );
//        assert( input.isEnd() );

//        assert( sync_acqs.size() == ref_sync_acqs.size() );
//        for ( int i = 0; i < ref_sync_acqs.size(); ++i ) {
//            const auto& sync_acq = sync_acqs.at( i );
//            CHECK( sync_acq == ref_sync_acqs.at( i ) );
//        }
