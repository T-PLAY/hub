

#include "test_common.hpp"

#include <core/Buffer.hpp>
#include <core/Traits.hpp>
#include <core/traits/Span.hpp>

TEST_CASE( "Buffer test" ) {
    TEST_BEGIN()

//#if CPP_VERSION >= 20
    using namespace hub;

    constexpr auto dataSize   = 1'000'000l; // 1Mo
    constexpr auto nIteration = 10;

    Data_t* rawData = new unsigned char[dataSize];
    for ( int i = 0; i < 256; ++i ) {
        rawData[i] = i;
    }
    rawData[dataSize - 1] = 93;

#    if CPP_VERSION >= 20
    std::span<Data_t, dataSize> rawSpan { rawData, rawData + dataSize };
#    endif

    //////////////////////////////////////////////////////////////////////////////////////////

    Buffer<Data_t, dataSize, BufferOption::DynamicMemory> myDynamicData;
    std::cout << "[dynamic] myDynamicData: " << myDynamicData << std::endl;
    auto dynamicStart = std::chrono::high_resolution_clock::now();
    for ( int i = 0; i < nIteration; ++i ) {
        std::copy(rawData, rawData + dataSize, myDynamicData.data());
#    if CPP_VERSION >= 20
        myDynamicData.setData( rawSpan );
#    endif
    }
    auto dynamicEnd = std::chrono::high_resolution_clock::now();
    auto dynamicDuration =
        std::chrono::duration_cast<std::chrono::microseconds>( dynamicEnd - dynamicStart ).count();
#    if CPP_VERSION >= 20
    CHECK( myDynamicData.getSpan() == rawSpan );
#    endif
//    assert(! memcmp(myDynamicData.data(), rawData, dataSize));
    std::cout << "[dynamic] myDynamicData: " << myDynamicData << std::endl;
    std::cout << "[dynamic] static copy duration: " << dynamicDuration / 1000.0 << " ms"
              << std::endl;
    std::cout << "[dynamic] static copy speed: "
              << nIteration * dataSize / ( dynamicDuration * 1000.0 ) << " GB/s" << std::endl;

    std::cout << "--------------------------------------------" << std::endl;

    static Buffer<Data_t, dataSize, BufferOption::StaticMemory> myStaticData;
    std::cout << "[static] myStaticData: " << myStaticData << std::endl;
    auto staticStart = std::chrono::high_resolution_clock::now();
    for ( int i = 0; i < nIteration; ++i ) {
#    if CPP_VERSION >= 20
        myStaticData.setData( rawSpan );
#    endif
        std::copy(rawData, rawData + dataSize, myDynamicData.data());
    }
    auto staticEnd = std::chrono::high_resolution_clock::now();
    auto staticDuration =
        std::chrono::duration_cast<std::chrono::microseconds>( staticEnd - staticStart ).count();
#    if CPP_VERSION >= 20
    CHECK( myStaticData.getSpan() == rawSpan );
#    endif
//    assert(! memcmp(myDynamicData.data(), rawData, dataSize));
    std::cout << "[static] myStaticData: " << myStaticData << std::endl;
    std::cout << "[static] static copy duration: " << staticDuration / 1000.0 << " ms" << std::endl;
    std::cout << "[static] static copy speed: "
              << nIteration * dataSize / ( staticDuration * 1000.0 ) << " GB/s" << std::endl;

    std::cout << "--------------------------------------------" << std::endl;

    const auto ratio = staticDuration / (double)dynamicDuration;
    // CHECK_VALUE( ratio, 3.0, 2.0, "DynamicData/StaticData", "/" );
    std::cout << "ratio : " << ratio << std::endl;
    std::cout << "check_decline ..." << std::endl;
    CHECK_DECLINE( ratio, "Buffer:DynamicData/StaticData", "/" );
    std::cout << "check_decline done" << std::endl;

    delete[] rawData;

    struct Random {
        int a;
        bool b;
        auto toString() const { return std::to_string( a ) + ":" + std::to_string( b ); }
    };

    constexpr Buffer<int, 2> buffer { 1, 2 };
    static_assert( buffer.get<0>() == 1 );
    static_assert( buffer.get<1>() == 2 );
    const Buffer<int, 2, BufferOption::DynamicMemory> buffer3 { 1, 2 };
    constexpr Buffer<char, 2> buffer2 { 'a', 'b' };
    constexpr Buffer<Random, 2, BufferOption::StaticMemory> buffer4 { Random { 5, true },
                                                                      Random { 2, false } };
    constexpr std::array<Random, 2> array { Random { 5, true }, Random { 2, false } };

    std::cout << "buffer4: " << buffer4 << std::endl;
    const Buffer<Random, 2, BufferOption::DynamicMemory> buffer5 { Random { 5, true },
                                                                   Random { 2, false } };
    std::cout << "buffer5: " << buffer5 << std::endl;

//#endif
    TEST_END()
}
