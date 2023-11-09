
//#define HUB_DEBUG_INPUT
//#define HUB_DEBUG_OUTPUT

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

//    constexpr auto dataSize = 10'000'000'000; // 1Go
//    constexpr auto dataSize = MAX_STACK_SIZE + 1; // 1Go
    constexpr auto dataSize = MAX_STACK_SIZE; // 1Go

//    using MyData = Data<dataSize, data::Option::Dynamic>;
    using MyData = Data<dataSize>;

//    std::cout << "MyData(): " << MyData() << std::endl;
    MyData myData;
    std::cout << "myData: " << myData << std::endl;

//    for (auto & v : myData.getSpan()) {
//        v = 5;
//    }
    auto span = myData.getSpan();
    auto start = std::chrono::high_resolution_clock::now();
    std::fill(span.begin(), span.end(), 5);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "myData: " << myData << std::endl;
    std::cout << "fill duration: " << duration << " us" << std::endl;
    std::cout << "output speed: " << dataSize / duration << " MB/s" << std::endl;

//    std::this_thread::sleep_for(std::chrono::seconds(5));


}
