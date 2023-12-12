
//#define HUB_DEBUG_INPUT
//#define HUB_DEBUG_OUTPUT

#include "test_common.hpp"

#include <core/Macros.hpp>
//#include <core/Vector.hpp>
#include <core/Traits.hpp>


class Lambda
{
  public:
    int a    = 0;
    float b  = 0.0f;
    double c = 0.0;
    std::vector<int> ds;
    std::string e;

    static constexpr auto Size = 5;

};


TEST_CASE( "Macros test" ) {
    TEST_BEGIN()

    static_assert(hub::sizeof_<int>() == 4);
    static_assert(hub::sizeof_<double>() == 8);
    static_assert(hub::sizeof_<int, int>() == 8);
    static_assert(hub::sizeof_<Lambda>() == 5);

    std::cout << "int: " << TYPE_NAME(int) << std::endl;
    std::cout << "vints: " << TYPE_NAME(std::vector<int>) << std::endl;

    std::cout << "string: " << TYPE_NAME(std::string) << std::endl;
    std::cout << "strings: " << TYPE_NAME(std::vector<std::string>) << std::endl;

    std::cout << "lambda: " << TYPE_NAME(Lambda) << std::endl;
    std::cout << "lambdas: " << TYPE_NAME(std::vector<Lambda>) << std::endl;

    TEST_END()
}
