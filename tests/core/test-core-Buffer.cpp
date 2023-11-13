
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT

#include "test_common.hpp"

#include <core/Buffer.hpp>
#include <core/Traits.hpp>
#include <core/traits/Span.hpp>

TEST_CASE( "Buffer test" ) {

//    std::cout << "FILE_NAME: " << FILE_NAME << std::endl;

    using namespace hub;

    //    constexpr auto dataSize = 10'000'000'000; // 10Go
//    constexpr auto dataSize = 1'000'000'000l; // 1Go
    constexpr auto dataSize = 1'000'000l; // 1Mo
                                             //    constexpr auto dataSize = MAX_STACK_SIZE; // 1Go
                                             //    constexpr auto dataSize = MAX_STACK_SIZE; // 1Go
    constexpr auto nIteration = 10;

    Data_t* rawData = new unsigned char[dataSize];
    for ( int i = 0; i < 256; ++i ) {
        rawData[i] = i;
    }
    rawData[dataSize - 1] = 93;

    std::span<Data_t, dataSize> rawSpan { rawData, rawData + dataSize };
    //    std::cout << "rawSpan: " << rawSpan << std::endl;

    //////////////////////////////////////////////////////////////////////////////////////////

    Buffer<Data_t, dataSize, BufferOption::DynamicMemory> myDynamicData;
    std::cout << "[dynamic] myDynamicData: " << myDynamicData << std::endl;
    auto dynamicStart = std::chrono::high_resolution_clock::now();
    for ( int i = 0; i < nIteration; ++i ) {
        myDynamicData.setData( rawSpan );
    }
    auto dynamicEnd = std::chrono::high_resolution_clock::now();
    auto dynamicDuration =
        std::chrono::duration_cast<std::chrono::microseconds>( dynamicEnd - dynamicStart ).count();
    CHECK( myDynamicData.getSpan() == rawSpan );
    std::cout << "[dynamic] myDynamicData: " << myDynamicData << std::endl;
    std::cout << "[dynamic] static copy duration: " << dynamicDuration / 1000.0 << " ms" << std::endl;
    std::cout << "[dynamic] static copy speed: "
              << nIteration * dataSize / ( dynamicDuration * 1000.0 ) << " GB/s" << std::endl;
    //    }

    std::cout << "--------------------------------------------" << std::endl;

    //    {
    static Buffer<Data_t, dataSize, BufferOption::StaticMemory> myStaticData;
    std::cout << "[static] myStaticData: " << myStaticData << std::endl;
    auto staticStart = std::chrono::high_resolution_clock::now();
    for ( int i = 0; i < nIteration; ++i ) {
        myStaticData.setData( rawSpan );
    }
    auto staticEnd = std::chrono::high_resolution_clock::now();
    auto staticDuration =
        std::chrono::duration_cast<std::chrono::microseconds>( staticEnd - staticStart ).count();
    CHECK( myStaticData.getSpan() == rawSpan );
//    auto span = myStaticData.getSpan();
    std::cout << "[static] myStaticData: " << myStaticData << std::endl;
    std::cout << "[static] static copy duration: " << staticDuration / 1000.0 << " ms" << std::endl;
    std::cout << "[static] static copy speed: "
              << nIteration * dataSize / ( staticDuration * 1000.0 ) << " GB/s" << std::endl;
    //    }

    std::cout << "--------------------------------------------" << std::endl;

    //    {

    const auto ratio = staticDuration / (double)dynamicDuration;
    CHECK_VALUE( ratio, 3.0, 2.0, "DynamicData/StaticData", "/" );
    //    std::this_thread::sleep_for(std::chrono::seconds(5));

    delete[] rawData;

//    constexpr auto dataStaticConstexpr = Buffer<1000, BufferOption::StaticMemory>();
    struct Random {
        int a;
        bool b;
        std::string toString() const {
            return std::to_string(a) + ":" + std::to_string(b);
        }
    };

    constexpr Buffer<int, 2> buffer{1, 2};
    static_assert(buffer.get<0>() == 1);
    static_assert(buffer.get<1>() == 2);
    const Buffer<int, 2, BufferOption::DynamicMemory> buffer3{1, 2};
//    static_assert(buffer3.get<0>() == 1);
//    static_assert(buffer3.get<1>() == 2);
    constexpr Buffer<char, 2> buffer2{'a', 'b'};
//    constexpr Buffer<Random, 2> buffer3{{5, true}, {2, false}};
    constexpr Buffer<Random, 2, BufferOption::StaticMemory> buffer4{Random{5, true}, Random{2, false}};
    constexpr std::array<Random, 2> array{Random{5, true}, Random{2, false}};

//    constexpr std::array<Random, 2> array2{5, true, 2, false};
    std::cout << "buffer4: " << buffer4 << std::endl;
//    Buffer<Random, 2, BufferOption::DynamicMemory> buffer5;
    const Buffer<Random, 2, BufferOption::DynamicMemory> buffer5{Random{5, true}, Random{2, false}};
    std::cout << "buffer5: " << buffer5 << std::endl;

//    constexpr auto dataStaticConstexpr = Buffer<2, BufferOption::StaticMemory>{"ab"};
    //    constexpr auto dataDynamicContexpr =  Buffer<1000, BufferOption::DynamicMemory>();
}
