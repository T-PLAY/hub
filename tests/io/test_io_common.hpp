
#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <ctime>
#include <execution>
#include <fstream>
#include <random>
#include <set>
#include <thread>
#include <tuple>

#include <core/Input.hpp>
#include <core/Output.hpp>
#include <core/io/InputOutput.hpp>
// #include <sensor/Acquisition.hpp>
// #include <sensor/InputSensor.hpp>
// #include <sensor/OutputSensor.hpp>

// #include "Macros.hpp"
// #include "Version.hpp"

template <typename T>
bool someEnd( const T& t ) {
    return t.isEnd();
}

template <typename T, typename... Inputs>
bool someEnd( const T& t, const Inputs&... args ) {
    //    return t.isEnd() && areEnd( args... );
    return t.isEnd() || someEnd( args... );
}

template <typename T>
bool anyEnd( const T& t ) {
    return t.isEnd();
}

template <typename T, typename... Inputs>
bool anyEnd( const T& t, const Inputs&... args ) {
    //    return t.isEnd() && areEnd( args... );
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

////    assert(input.isEnd());
//}
// constexpr static size_t s_dataSize   = 1'000'000'000; // 1Go
constexpr static size_t s_dataSize = 1'000'000; // 1Mo
constexpr static auto s_nIteration = 100;
static std::vector<hub::Data_t> s_testData;
static std::vector<hub::Data_t> s_testData_read;

static auto generateTestData() {
    if ( s_testData.empty() ) {
        s_testData.resize( s_dataSize );
        s_testData_read.resize( s_dataSize );
        std::fill( s_testData.begin(), s_testData.end(), rand() );
        //        memset(s_testData.data(), 55, s_dataSize);
    }
    return std::tuple<const hub::Data_t*, hub::Size_t>( s_testData.data(), s_testData.size() );
}

template <class Output, class Input>
static auto inputOutputBench( Output& output, Input& input ) {
    //    std::array<hub::Data_t, dataSize> array;
    //    std::vector<hub::Data_t> vector( s_dataSize );
    //    std::fill( vector.begin(), vector.end(), rand() );

    //    hub::Data_t* data = s_testData.data();
    //    hub::Size_t size  = s_testData.size();

    const auto& [data, size] = generateTestData();

    //    std::data<hub::Data_t, dataSize> array_read;
    //    std::vector<hub::Data_t> s_testData_read( s_dataSize );
    static std::atomic<bool> startSignal = false;

    std::thread thread( [&]() {
        while ( !startSignal ) {};
        for ( int i = 0; i < s_nIteration; ++i ) {
            //            memset(data, i, size);
            output.write( data, size );
        }
    } );

    std::thread thread2( [&]() {
        while ( !startSignal ) {};
        for ( int i = 0; i < s_nIteration; ++i ) {
            //            output.write( data, size );
            input.read( s_testData_read.data(), size );
#ifdef DEBUG
            assert( s_testData == s_testData_read );
//            assert(mem(s_testData_read.data(), ))
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
    //    struct Report {
    //        duration
    //    };

    //    std::cout << "------------------------------------" << std::endl;
    //    std::cout << "[inputOutputBench] writing/reading rate: " << std::to_string(
    //    gigaBytePerSecond )
    //              << " Go/s" << std::endl;
    //    std::cout << "[inputOutputBench] total time: " << durationInNanoSecond / 1000'000.0 << "
    //    ms"
    //              << std::endl;
    //    return std::make_tuple<>(durationInMicroSecond, gigaBytePerSecond);
    return std::tuple<double, double>( durationInNanoSecond / 1'000'000.0, gigaBytePerSecond );
}

template <class Output, class Input>
static auto inputOutputBench( Output& output, std::vector<Input>& inputs ) {
    //    std::array<hub::Data_t, dataSize> array;
    //    std::vector<hub::Data_t> vector( s_dataSize );
    //    std::fill( vector.begin(), vector.end(), rand() );

    //    hub::Data_t* data = vector.data();
    //    hub::Size_t size  = vector.size();

    const auto& [data, size] = generateTestData();

    //    std::data<hub::Data_t, dataSize> array_read;
    //    std::vector<hub::Data_t> vector_read( s_dataSize );
    static std::atomic<bool> startSignal = false;

    std::thread thread( [&]() {
        while ( !startSignal ) {};

        for ( int i = 0; i < s_nIteration; ++i ) {
            output.write( data, size );
        }
    } );

//        std::thread thread2( [&]() {
////                    std::for_each( std::execution::seq, inputs.begin(), inputs.end(), []( auto&
////    /input /                ) {
//            std::for_each( std::execution::par, inputs.begin(), inputs.end(), []( auto& input ) {
//                while ( !startSignal ) {};

//                for ( int i = 0; i < s_nIteration; ++i ) {
//                    //        output.write( data, size );
//                    input.read( s_testData_read.data(), s_dataSize );
//     #ifdef DEBUG
//                    assert( s_testData == s_testData_read );
//     #endif
//                }
//            } );
//        } );

    std::thread thread2( [&]() {
        //                std::for_each( std::execution::seq, inputs.begin(), inputs.end(), [](
        //                auto& input ) {
        while ( !startSignal ) {};
        //        std::for_each( std::execution::par, inputs.begin(), inputs.end(), []( auto& input
        //        ) {
        for ( int i = 0; i < s_nIteration; ++i ) {
            for ( auto& input : inputs ) {
                //        output.write( data, size );
                input.read( s_testData_read.data(), s_dataSize );
#ifdef DEBUG
                assert( s_testData == s_testData_read );
#endif
            }
        }
        // );
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
        //        ( ( 1 + inputs.size() ) * s_nIteration * s_dataSize ) /
        //        (double)durationInNanoSecond;
        (( 1 + inputs.size()) * s_nIteration * s_dataSize ) / (double)durationInNanoSecond;
    //    struct Report {
    //        duration
    //    };

    //    std::cout << "------------------------------------" << std::endl;
    //    std::cout << "[inputOutputBench] writing/reading rate: " << std::to_string(
    //    gigaBytePerSecond )
    //              << " Go/s" << std::endl;
    //    std::cout << "[inputOutputBench] total time: " << durationInNanoSecond / 1000'000.0 << "
    //    ms"
    //              << std::endl;
    //    return std::make_tuple<>(durationInMicroSecond, gigaBytePerSecond);
    return std::tuple<double, double>( durationInNanoSecond / 1'000'000.0, gigaBytePerSecond );
}
