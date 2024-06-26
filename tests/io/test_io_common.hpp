
#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <ctime>
#include <execution>
#include <filesystem>
#include <fstream>
#include <random>
#include <set>
#include <thread>
#include <tuple>

#include <core/Input.hpp>
#include <core/Output.hpp>
#include <core/io/InputOutput.hpp>

#include <io/Stream.hpp>
#include <server/Server.hpp>

#define SERVER_IP "127.0.0.1"

#define INIT_SERVER                                                 \
    const auto fileName = "hubServerPort.txt";                      \
    int SERVER_PORT;                                                \
    std::unique_ptr<hub::Server> server;                            \
    if ( std::filesystem::exists( fileName ) ) {                    \
        std::ifstream file( fileName );                             \
        file >> SERVER_PORT;                                        \
        file.close();                                               \
    }                                                               \
    else {                                                          \
        SERVER_PORT = GET_RANDOM_PORT;                              \
        server      = std::make_unique<hub::Server>( SERVER_PORT ); \
        server->asyncRun();                                         \
    }

template <typename T>
bool someEnd( const T& t ) {
    return t.isEnd();
}

template <typename T, typename... Inputs>
bool someEnd( const T& t, const Inputs&... args ) {
    return t.isEnd() || someEnd( args... );
}

template <typename T>
bool anyEnd( const T& t ) {
    return t.isEnd();
}

template <typename T, typename... Inputs>
bool anyEnd( const T& t, const Inputs&... args ) {
    return t.isEnd() && anyEnd( args... );
}

template <class Input>
void clearAll( Input& input ) {
    input.clear();
}
template <class Input, class... Inputs>
void clearAll( Input& input, Inputs&... inputs ) {
    input.clear();
    if ( sizeof...( Inputs ) > 0 ) { clearAll( inputs... ); }
}

////    CHECK(input.isEnd());
constexpr static size_t s_dataSize = 1'000'000; // 1Mo
constexpr static auto s_nIteration = 100;
static std::vector<hub::Data_t> s_testData;
static std::vector<hub::Data_t> s_testData_read;

static auto generateTestData() {
    if ( s_testData.empty() ) {
        s_testData.resize( s_dataSize );
        s_testData_read.resize( s_dataSize );
        std::generate( s_testData.begin(), s_testData.end(), rand );
    }
    return std::tuple<const hub::Data_t*, hub::Size_t>( s_testData.data(), s_testData.size() );
}

static const hub::io::Header s_header_ref { s_dataSize };

#define TEST_IO_HEADER s_header_ref

template <class Input, class Output>
static auto inputOutputBench( Input& input, Output& output, std::string verbose = "" ) {

    const auto& [dataGen, sizeGen] = generateTestData();
    const hub::Data_t* data        = dataGen;
    const hub::Size_t size         = sizeGen;

    static std::atomic<bool> startSignal = false;

    std::thread thread( [&]() {
        while ( !startSignal ) {};
        for ( int i = 0; i < s_nIteration; ++i ) {
            output.write( data, size );
        }
    } );

    std::thread thread2( [&]() {
        while ( !startSignal ) {};
        for ( int i = 0; i < s_nIteration; ++i ) {
            input.read( s_testData_read.data(), size );
#ifdef DEBUG
            CHECK( s_testData == s_testData_read );
#endif
        }
    } );

    const auto startClock = std::chrono::high_resolution_clock::now();

    startSignal = true;

    thread.join();
    thread2.join();

    const auto endClock = std::chrono::high_resolution_clock::now();

    CHECK( s_testData == s_testData_read );

    const auto durationInNanoSecond =
        std::chrono::duration_cast<std::chrono::nanoseconds>( endClock - startClock ).count();
    const auto gigaBytePerSecond = ( 2 * s_nIteration * s_dataSize ) / (double)durationInNanoSecond;

    if ( !verbose.empty() ) {
        std::cout << "[InputOutput:" << verbose
                  << "] writing/reading rate: " << std::to_string( gigaBytePerSecond ) << " Go/s"
                  << std::endl;
        std::cout << "[InputOutput:" << verbose
                  << "] total time: " << durationInNanoSecond / 1'000'000.0 << " ms" << std::endl;
    }

    return std::tuple<double, double>( durationInNanoSecond / 1'000'000.0, gigaBytePerSecond );
}

template <class Input, class Output>
static auto inputOutputBench( std::vector<Input>& inputs,
                              std::vector<Output>& outputs,
                              std::string verbose = "" ) {

    const auto& [dataGen, sizeGen] = generateTestData();
    const hub::Data_t* data        = dataGen;
    const hub::Size_t size         = sizeGen;

    static std::atomic<bool> startSignal = false;

    std::thread thread( [&]() {
        while ( !startSignal ) {};

        for ( int i = 0; i < s_nIteration; ++i ) {
            for ( auto& output : outputs ) {
                output.write( data, size );
            }
        }
    } );

    std::thread thread2( [&]() {
        while ( !startSignal ) {};

        for ( int i = 0; i < s_nIteration; ++i ) {
            for ( auto& input : inputs ) {
                input.read( s_testData_read.data(), s_dataSize );
#ifdef DEBUG
                CHECK( s_testData == s_testData_read );
#endif
            }
        }
    } );

    const auto startClock = std::chrono::high_resolution_clock::now();

    startSignal = true;

    thread.join();
    thread2.join();

    const auto endClock = std::chrono::high_resolution_clock::now();

    CHECK( s_testData == s_testData_read );

    const auto durationInNanoSecond =
        std::chrono::duration_cast<std::chrono::nanoseconds>( endClock - startClock ).count();
    const auto gigaBytePerSecond =
        ( ( outputs.size() + inputs.size() ) * s_nIteration * s_dataSize ) /
        (double)durationInNanoSecond;

    if ( !verbose.empty() ) {
        std::cout << "[InputOutput:" << verbose
                  << "] writing/reading rate: " << std::to_string( gigaBytePerSecond ) << " Go/s"
                  << std::endl;
        std::cout << "[InputOutput:" << verbose
                  << "] total time: " << durationInNanoSecond / 1'000'000.0 << " ms" << std::endl;
    }

    return std::tuple<double, double>( durationInNanoSecond / 1'000'000.0, gigaBytePerSecond );
}
