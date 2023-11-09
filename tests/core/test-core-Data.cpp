
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT

#include "test_common.hpp"

#include <core/Data.hpp>
#include <core/Vector.hpp>

class Lambda
{
  public:
    int a    = 0;
    float b  = 0.0f;
    double c = 0.0;
    std::vector<int> ds;
    std::string e;

    template <class Serial>
    void serialize( Serial& serial ) {
        serial( a, b, c, ds, e );
    }

    friend std::ostream& operator<<( std::ostream& os, const Lambda& lambda ) {
        os << lambda.a << " " << lambda.b << " " << lambda.c << " " << lambda.ds << " " << lambda.e;
        return os;
    }
    bool operator==( const Lambda& lambda ) const {
        return a == lambda.a && b == lambda.b && c == lambda.c && ds == lambda.ds && e == lambda.e;
    }
};

TEST_CASE( "Data test" ) {
    using namespace hub;

    //        constexpr auto dataSize = 10'000'000'000; // 1Go
            constexpr auto dataSize = 1'000'000'000; // 1Go
//    constexpr auto dataSize = MAX_STACK_SIZE; // 1Go
                                              //    constexpr auto dataSize = MAX_STACK_SIZE; // 1Go
    constexpr auto nIteration = 1;

    Data_t* rawData = new unsigned char[dataSize];
    for ( int i = 0; i < 256; ++i ) {
        rawData[i] = i;
    }
    rawData[dataSize - 1] = 93;

    std::span<Data_t, dataSize> rawSpan { rawData, rawData + dataSize };
    std::cout << "rawSpan: " << rawSpan << std::endl;

    //    {
    static Data<dataSize, data::Option::Static> myStaticData;
    std::cout << "[static] myStaticData: " << myStaticData << std::endl;
    auto staticStart = std::chrono::high_resolution_clock::now();
    for ( int i = 0; i < nIteration; ++i ) {
        myStaticData.setData( rawSpan );
    }
    auto staticEnd = std::chrono::high_resolution_clock::now();
    auto staticDuration =
        std::chrono::duration_cast<std::chrono::microseconds>( staticEnd - staticStart ).count();
    CHECK( myStaticData.getSpan() == rawSpan );
    std::cout << "[static] myStaticData: " << myStaticData << std::endl;
    std::cout << "[static] static copy duration: " << staticDuration << " us" << std::endl;
    std::cout << "[static] static copy speed: "
              << nIteration * dataSize / ( staticDuration * 1000.0 ) << " GB/s" << std::endl;
    //    }

    std::cout << "--------------------------------------------" << std::endl;

    //    {
    Data<dataSize, data::Option::Dynamic> myDynamicData;
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
    std::cout << "[dynamic] static copy duration: " << dynamicDuration << " us" << std::endl;
    std::cout << "[dynamic] static copy speed: "
              << nIteration * dataSize / ( dynamicDuration * 1000.0 ) << " GB/s" << std::endl;
    //    }

    const auto ratio = staticDuration / (double)dynamicDuration;
    CHECK_VALUE( ratio, 3.0, 2.0, "DynamicData/StaticData", "/" );
    //    std::this_thread::sleep_for(std::chrono::seconds(5));

    delete[] rawData;
}
