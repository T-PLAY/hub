
//#define HUB_DEBUG_INPUT
//#define HUB_DEBUG_OUTPUT

#include "test_common.hpp"
#include "core/test-core-common.hpp"

// #include <core/InputOutput.hpp>
// #include <core/io/InputOutputZppBits.hpp>
// #include <core/io/

#include <core/io/Archive.hpp>
#include <core/io/Book.hpp>
#include <core/io/InputOutputI.hpp>
#include <core/io/InputOutputImpl.hpp>
#include <core/io/InputOutputZppBits.hpp>
#include <core/io/iosBase.hpp>

namespace testCoreInputOutput {

struct Data {
    //    std::string_view name;
    //    std::string name;
    int age;
    double size;

    //    static struct {} notReadable;
    static struct {
    } packable;

    //    bool operator==( const Data& data ) const { return name == data.name && age == data.age; }
    bool operator==( const Data& data ) const { return age == data.age; }
    friend std::ostream& operator<<( std::ostream& os, const Data& data );
};
//static_assert(hub::serializable_v<Data>);
static_assert( sizeof( int ) == 4 );
static_assert( sizeof( double ) == 8 );
// static_assert( sizeof( Data ) == 16 );

std::ostream& operator<<( std::ostream& os, const Data& data ) {
    os << data.age;
    return os;
}

static constexpr size_t s_dataSize = sizeof( int );

 constexpr size_t s_nReadWritePerIteration = 100'000;
//static constexpr size_t s_nReadWritePerIteration = 100;
static constexpr int s_nIteration                = 2;
static constexpr size_t s_nReadWrite             = s_nReadWritePerIteration * s_nIteration;

enum class IOImpls { Archive = 0, Book, Count };
static constexpr auto s_nIOImpl = (int)IOImpls::Count;
static constexpr std::string_view ioImplNames[s_nIOImpl] { "Archive", "Book" };

static double s_megaReadWritePerSeconds[s_nIOImpl];
static double s_gigaBytePerSeconds[s_nIOImpl];
static long long s_durations[s_nIOImpl];

template <class IO_Impl, class Data>
auto processBench( IO_Impl& ioImpl, const Data& data, int i ) {

    Data data_read;

    const auto startClock = std::chrono::high_resolution_clock::now();
    for ( int iIter = 0; iIter < s_nIteration; ++iIter ) {

        size_t iReadWrite = 0;
        while ( iReadWrite < s_nReadWritePerIteration ) {
//            const int data_int = 5;
            data_read = 0;
//            data = 0;
            ioImpl.write( data );
//            assert( !ioImpl.isEnd() );

            ioImpl.read( data_read );
//            assert( data_read == data );
            if (data_read != data)  {
                std::cerr << data_read << " != " << data << std::endl;
            }
//            assert( ioImpl.isEnd() );

            ++iReadWrite;
        }
    }
    const auto endClock = std::chrono::high_resolution_clock::now();
    s_durations[i] =
        std::chrono::duration_cast<std::chrono::microseconds>( endClock - startClock ).count();
    assert(s_durations[i] > 0);

    s_megaReadWritePerSeconds[i] = 2.0 * (double)s_nReadWrite / s_durations[i];
    assert(s_megaReadWritePerSeconds[i] > 0);
    s_gigaBytePerSeconds[i]      = s_megaReadWritePerSeconds[i] * s_dataSize / 1000.0;
    assert(s_gigaBytePerSeconds[i] > 0);
};

} // namespace testCoreInputOutput

TEST_CASE( "InputOutput test" ) {
    using namespace testCoreInputOutput;
    //            InputOutputImpl<datasSize> inputOutput;
    //    std::vector<unsigned char> dataV;
    //    constexpr int nData = 1;
    //    std::tuple<int> datas;
    //    std::get<0>(datas) = 5;
    //    int data_int = 5;
    //    int data_int_read;

    //    const auto data             = Data { 30, 2.0 };
    //    const int data             = 5;
    //    const size_t dataSize = nReadWrite * sizeof( data );
    //    constexpr int nIteration = 1;
    std::cout << "nb read/write: " << std::to_string( s_nReadWrite / 1'000'000.0 ) << " M"
              << std::endl;
    std::cout << "read/write size: " << std::to_string( s_dataSize * s_nReadWrite / 1'000'000.0 )
              << " Mo" << std::endl;
    std::cout << std::endl;

//    int data = 5;

//    hub::io::Archive archive;
//    processBench( archive, data, 0 );

////    hub::io::Book<> book;
////    processBench( book, data, 1 );

//    for ( int iImpl = 0; iImpl < s_nIOImpl; ++iImpl ) {

//        const auto& implName = ioImplNames[iImpl];
//        std::cout << "------------------------------------" << std::endl;
//        std::cout << "[" << implName
//                  << "] writing/reading rate: " << std::to_string( s_megaReadWritePerSeconds[iImpl] )
//                  << " MegaReadWrite/s, " << std::to_string( s_gigaBytePerSeconds[iImpl] ) << " Go/s";
//        if ( iImpl == s_nIOImpl - 1 ) {
//            std::cout << " (" << 100 * s_megaReadWritePerSeconds[1] / s_megaReadWritePerSeconds[0]
//                      << " %)";
//        }
//        std::cout << std::endl;

//        std::cout << "[" << implName << "] total time: " << s_durations[iImpl] / 1000.0 << " ms";
//        if ( iImpl == s_nIOImpl - 1 ) {
//            std::cout << " (" << 100 * s_durations[0] / s_durations[1] << " %)";
//        }
//        std::cout << std::endl;
//    }

//    //    std::cout << "[Archive] writing rate: " << std::to_string(
//    //    megaReadWritePerSecondArchive )
//    //              << " MegaWrite/s, " << std::to_string( gigaBytePerSecondArchive ) << " Go/s"
//    //              << std::endl;
//    //    std::cout << "[Archive] total time: " << durationArchive / 1000.0 << " ms" <<
//    //    std::endl;

//    //    Data a2 { 29, 1.0 };
//    //    archive.write( a2 );
//    //    Data a2_read;
//    //    archive.read( a2_read );
//    //    assert( a2 == a2_read );

//    ///////////////////////////////////

//    //    book.write( data_int );
//    //    book.read( data_int_read );
//    //    assert( data_int_read == data_int );

//    //    hub::io::input::InputI& inputBook    = book;
//    //    hub::io::input::BasicInputI& basicInputBook    = book;
//    //    hub::io::InputOutputI & inputOutputBook = book;

//    return;

//    ////////////////////////////////////

//    hub::io::Archive<hub::io::InputOutputImpl> archive2;
//    //    hub::io::InputOutputI& inputOutput = archive2;
//    archive2.write( 5 );
//    //    output.write(5);
//    int b;
//    archive2.read( b );
//    assert( b == 5 );

//    /////////////////////////////////////////////

//    hub::io::Archive<hub::io::InputOutputZppBits> archive3;
//    archive3.write( 5 );
//    //    output.write(5);
//    int c;
//    archive3.read( c );
//    assert( c == 5 );

//    //    auto inputOutput = hub::InputOutput(dataV);
//    //    hub::InputOutput inputOutput;
//    //    auto & input = inputOutput;
//    //    auto & output = inputOutput;

//    std::vector<hub::io::InputOutputI*> inputOutputs;
//    inputOutputs.push_back( (hub::io::InputOutputI*)&archive2 );
//    inputOutputs.push_back( (hub::io::InputOutputI*)&archive3 );

//    return;
//    std::cout << "#################################################################################"
//                 "###############"
//              << std::endl;

//    //    enum class InputOutputImplName { Impl = 0, ZppBits, COUNT };
//    enum class InputOutputImplName { Impl = 0, ZppBits, COUNT };
//    //    static_assert( static_cast<int>( InputOutputImplName::COUNT ) == 2 );
//    constexpr auto nImpl = static_cast<int>( InputOutputImplName::COUNT );
//    static constexpr std::string_view implNames[nImpl] { "Impl", "ZppBits" };
//    static_assert( InputOutputImplName( 0 ) == InputOutputImplName::Impl );
//    //    static_assert(InputOutputImplName(1) == InputOutputImplName::ZppBits);

//    double megaWritePerSecond[nImpl];
//    double megaWriteGigabytePerSecond[nImpl];
//    double megaReadPerSecond[nImpl];
//    double megaReadGigabytePerSecond[nImpl];

//    long long durationOutput[nImpl] { 0 };
//    long long durationInput[nImpl] { 0 };
//    long long duration[nImpl] { 0 };

    //    for ( int iImpl = 0; iImpl < nImpl; ++iImpl ) {
    //        InputOutputImplName impl = InputOutputImplName( iImpl );
    //        const auto& implName     = implNames[iImpl];

    //        auto& inputOutput                = *inputOutputs[iImpl];
    //        hub::io::input::InputI& input    = inputOutput;
    //        hub::io::output::OutputI& output = inputOutput;

    //        for ( int iIter = 0; iIter < nIteration; ++iIter ) {

    //            //            InputOutputImpl<datasSize> inputOutput;

    //            auto startClock = std::chrono::high_resolution_clock::now();

    //            size_t nWrite         = 0;
    //            auto startOutputClock = std::chrono::high_resolution_clock::now();
    //            while ( nWrite < nReadWrite ) {
    //                output.write( data );
    //                ++nWrite;
    //            }
    //            auto endOutputClock = std::chrono::high_resolution_clock::now();

    //            size_t nRead = 0;
    //            Data data_read;
    ////            int data_read;
    //            auto startInputClock = std::chrono::high_resolution_clock::now();
    //            while ( nRead < nReadWrite ) {
    //                input.read( data_read );
    //                assert( data_read == data );
    //                //                if ( data_read != data ) {
    //                //                    assert(false);
    //                //                    std::cerr << data_read << " != " << data <<
    //                std::endl;
    //                //                    continue;
    //                //                }
    //                ++nRead;
    //            }
    //            auto endInputClock = std::chrono::high_resolution_clock::now();
    //            auto endClock      = std::chrono::high_resolution_clock::now();

    //            assert( nWrite == nReadWrite );
    //            assert( nRead == nReadWrite );

    //            durationOutput[iImpl] +=
    //            std::chrono::duration_cast<std::chrono::microseconds>(
    //                                         endOutputClock - startOutputClock )
    //                                         .count();
    //            durationInput[iImpl] += std::chrono::duration_cast<std::chrono::microseconds>(
    //                                        endInputClock - startInputClock )
    //                                        .count();
    //            duration[iImpl] +=
    //                std::chrono::duration_cast<std::chrono::microseconds>( endClock -
    //                startClock )
    //                    .count();
    //        }

    //        megaWritePerSecond[iImpl]         = (double)nReadWrite * nIteration /
    //        durationOutput[iImpl]; megaWriteGigabytePerSecond[iImpl] =
    //        megaWritePerSecond[iImpl] * sizeof( Data ) / 1000.0;

    //        megaReadPerSecond[iImpl]         = (double)nReadWrite * nIteration /
    //        durationInput[iImpl]; megaReadGigabytePerSecond[iImpl] = megaReadPerSecond[iImpl]
    //        * sizeof( Data ) / 1000.0;

    //    }

    //    for ( int iImpl = 0; iImpl < nImpl; ++iImpl ) {
    //        const auto& implName = implNames[iImpl];
    //        std::cout << "------------------------------------" << std::endl;
    //        std::cout << "[" << implName
    //                  << "] writing rate: " << std::to_string( megaWritePerSecond[iImpl] )
    //                  << " MegaWrite/s, " << std::to_string( megaWriteGigabytePerSecond[iImpl]
    //                  )
    //                  << " Go/s";
    //        if ( iImpl == nImpl - 1 ) {
    //            std::cout << " (" << 100 * megaWritePerSecond[1] / megaWritePerSecond[0] << "
    //            %)";
    //        }
    //        std::cout << std::endl;
    //        std::cout << "[" << implName
    //                  << "] reading rate: " << std::to_string( megaReadPerSecond[iImpl] )
    //                  << " MegaRead/s, " << std::to_string( megaReadGigabytePerSecond[iImpl] )
    //                  << " Go/s";
    //        if ( iImpl == nImpl - 1 ) {
    //            std::cout << " (" << 100 * megaReadPerSecond[1] / megaReadPerSecond[0] << "
    //            %)";
    //        }
    //        std::cout << std::endl;
    //        std::cout << "[" << implName << "] write time: " << durationOutput[iImpl] / 1000.0
    //        << " ms"; if ( iImpl == nImpl - 1 ) {
    //            std::cout << " (" << 100 * durationOutput[0] / durationOutput[1] << " %)";
    //        }
    //        std::cout << std::endl;
    //        std::cout << "[" << implName << "] read time: " << durationInput[iImpl] / 1000.0
    //        << " ms"; if ( iImpl == nImpl - 1 ) {
    //            std::cout << " (" << 100 * durationInput[0] / durationInput[1] << " %)";
    //        }
    //        std::cout << std::endl;
    //        std::cout << "[" << implName << "] total time: " << duration[iImpl] / 1000.0 << "
    //        ms"; if ( iImpl == nImpl - 1 ) { std::cout << " (" << 100 * duration[0] /
    //        duration[1] << "
    //        %)"; } std::cout << std::endl;
    //    }
}
