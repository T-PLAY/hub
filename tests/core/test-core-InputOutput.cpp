
#define HUB_DEBUG_INPUT
#define HUB_DEBUG_OUTPUT

#include "test-core-common.hpp"
#include "test_common.hpp"

// #include <core/InputOutput.hpp>
// #include <core/io/InputOutputZppBits.hpp>
// #include <core/io/

#include <core/io/Archive.hpp>
#include <core/io/InputOutputI.hpp>
#include <core/io/InputOutputImpl.hpp>
#include <core/io/InputOutputZppBits.hpp>

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
static_assert( sizeof( int ) == 4 );
static_assert( sizeof( double ) == 8 );
// static_assert( sizeof( Data ) == 16 );

std::ostream& operator<<( std::ostream& os, const Data& data ) {
    os << data.age;
    return os;
}

} // namespace testCoreInputOutput

TEST_CASE( "InputOutput test" ) {
    using namespace testCoreInputOutput;
    //            InputOutputImpl<datasSize> inputOutput;
    //    std::vector<unsigned char> dataV;

    hub::io::Archive archive;
    //    hub::io::InputOutputI & inputOutput = archive;
    hub::io::input::InputI& input    = archive;
    hub::io::output::OutputI& output = archive;
    output.write( 5 );
    int a;
    input.read( a );
    assert( a == 5 );

    Data a2 { 29, 1.0 };
    output.write( a2 );
    Data a2_read;
    input.read( a2_read );
    assert( a2 == a2_read );

    ////////////////////////////////////

    hub::io::Archive<hub::io::InputOutputImpl> archive2;
    hub::io::InputOutputI& inputOutput = archive;
    inputOutput.write( 5 );
    //    output.write(5);
    int b;
    inputOutput.read( b );
    assert( b == 5 );

    /////////////////////////////////////////////

    hub::io::Archive<hub::io::InputOutputZppBits> archive3;
    archive3.write( 5 );
    //    output.write(5);
    int c;
    archive3.read( c );
    assert( c == 5 );

    //    auto inputOutput = hub::InputOutput(dataV);
    //    hub::InputOutput inputOutput;
    //    auto & input = inputOutput;
    //    auto & output = inputOutput;

    std::vector<hub::io::InputOutputI*> inputOutputs;
    inputOutputs.push_back( (hub::io::InputOutputI*)&archive2 );
    inputOutputs.push_back( (hub::io::InputOutputI*)&archive3 );

    return;
    std::cout << "#################################################################################"
                 "###############"
              << std::endl;

    //    constexpr size_t nReadWrite = 100;
    constexpr size_t nReadWrite = 100'000;
    const auto data             = Data { 30, 2.0 };
    //    const int data             = 5;
    constexpr size_t datasSize = nReadWrite * sizeof( data );
    constexpr int nIteration   = 10;
    //    constexpr int nIteration = 1;
    std::cout << "nb read/write: " << std::to_string( nReadWrite * nIteration / 1'000'000.0 )
              << " M" << std::endl;
    std::cout << "read/write size: " << std::to_string( datasSize * nIteration / 1'000'000.0 )
              << " Mo" << std::endl;
    std::cout << std::endl;

    //    enum class InputOutputImplName { Impl = 0, ZppBits, COUNT };
    enum class InputOutputImplName { Impl = 0, ZppBits, COUNT };
    //    static_assert( static_cast<int>( InputOutputImplName::COUNT ) == 2 );
    constexpr auto nImpl = static_cast<int>( InputOutputImplName::COUNT );
    static constexpr std::string_view implNames[nImpl] { "Impl", "ZppBits" };
    static_assert( InputOutputImplName( 0 ) == InputOutputImplName::Impl );
    //    static_assert(InputOutputImplName(1) == InputOutputImplName::ZppBits);

    double megaWritePerSecond[nImpl];
    double megaWriteGigabytePerSecond[nImpl];
    double megaReadPerSecond[nImpl];
    double megaReadGigabytePerSecond[nImpl];

    long long durationOutput[nImpl] { 0 };
    long long durationInput[nImpl] { 0 };
    long long duration[nImpl] { 0 };

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
    //                //                    std::cerr << data_read << " != " << data << std::endl;
    //                //                    continue;
    //                //                }
    //                ++nRead;
    //            }
    //            auto endInputClock = std::chrono::high_resolution_clock::now();
    //            auto endClock      = std::chrono::high_resolution_clock::now();

    //            assert( nWrite == nReadWrite );
    //            assert( nRead == nReadWrite );

    //            durationOutput[iImpl] += std::chrono::duration_cast<std::chrono::microseconds>(
    //                                         endOutputClock - startOutputClock )
    //                                         .count();
    //            durationInput[iImpl] += std::chrono::duration_cast<std::chrono::microseconds>(
    //                                        endInputClock - startInputClock )
    //                                        .count();
    //            duration[iImpl] +=
    //                std::chrono::duration_cast<std::chrono::microseconds>( endClock - startClock )
    //                    .count();
    //        }

    //        megaWritePerSecond[iImpl]         = (double)nReadWrite * nIteration /
    //        durationOutput[iImpl]; megaWriteGigabytePerSecond[iImpl] = megaWritePerSecond[iImpl] *
    //        sizeof( Data ) / 1000.0;

    //        megaReadPerSecond[iImpl]         = (double)nReadWrite * nIteration /
    //        durationInput[iImpl]; megaReadGigabytePerSecond[iImpl] = megaReadPerSecond[iImpl] *
    //        sizeof( Data ) / 1000.0;

    //    }

    //    for ( int iImpl = 0; iImpl < nImpl; ++iImpl ) {
    //        const auto& implName = implNames[iImpl];
    //        std::cout << "------------------------------------" << std::endl;
    //        std::cout << "[" << implName
    //                  << "] writing rate: " << std::to_string( megaWritePerSecond[iImpl] )
    //                  << " MegaWrite/s, " << std::to_string( megaWriteGigabytePerSecond[iImpl] )
    //                  << " Go/s";
    //        if ( iImpl == nImpl - 1 ) {
    //            std::cout << " (" << 100 * megaWritePerSecond[1] / megaWritePerSecond[0] << " %)";
    //        }
    //        std::cout << std::endl;
    //        std::cout << "[" << implName
    //                  << "] reading rate: " << std::to_string( megaReadPerSecond[iImpl] )
    //                  << " MegaRead/s, " << std::to_string( megaReadGigabytePerSecond[iImpl] )
    //                  << " Go/s";
    //        if ( iImpl == nImpl - 1 ) {
    //            std::cout << " (" << 100 * megaReadPerSecond[1] / megaReadPerSecond[0] << " %)";
    //        }
    //        std::cout << std::endl;
    //        std::cout << "[" << implName << "] write time: " << durationOutput[iImpl] / 1000.0 <<
    //        " ms"; if ( iImpl == nImpl - 1 ) {
    //            std::cout << " (" << 100 * durationOutput[0] / durationOutput[1] << " %)";
    //        }
    //        std::cout << std::endl;
    //        std::cout << "[" << implName << "] read time: " << durationInput[iImpl] / 1000.0 << "
    //        ms"; if ( iImpl == nImpl - 1 ) {
    //            std::cout << " (" << 100 * durationInput[0] / durationInput[1] << " %)";
    //        }
    //        std::cout << std::endl;
    //        std::cout << "[" << implName << "] total time: " << duration[iImpl] / 1000.0 << " ms";
    //        if ( iImpl == nImpl - 1 ) { std::cout << " (" << 100 * duration[0] / duration[1] << "
    //        %)"; } std::cout << std::endl;
    //    }
}
