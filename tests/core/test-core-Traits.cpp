
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT

#include "test_common.hpp"

#include <core/Traits.hpp>
#include <array>
#include <span>

class Lambda
{
  public:
    int a    = 0;
    float b  = 0.0f;
    double c = 0.0;
    std::vector<int> ds;
    std::string e;

    friend std::ostream& operator<<( std::ostream& os, const Lambda& lambda ) {
        os << lambda.a << " " << lambda.b << " " << lambda.c << " " << lambda.ds << " " << lambda.e;
        return os;
    }
};

TEST_CASE( "Traits test" ) {
    TEST_BEGIN()

    //    using namespace hub;

    const std::vector<int> vints { 1, 2, 3 };
    std::cout << "vints: " << toString( vints ) << std::endl;
    std::cout << "vints: " << vints << std::endl;
    std::cout << std::endl;

    const std::vector<char> vchar { 'a', 'b', 'c' };
    std::cout << "vchar: " << toString( vchar ) << std::endl;
    std::cout << "vchar: " << vchar << std::endl;
    std::cout << std::endl;

    const std::array<int, 5> array { 1, 2, 3, 4, 5 };
    std::cout << "array: " << toString( array ) << std::endl;
    std::cout << "array: " << array << std::endl;
    std::cout << std::endl;

    const std::span span { array };
    std::cout << "span: " << toString( span ) << std::endl;
    std::cout << "span: " << span << std::endl;
    std::cout << std::endl;

    const std::string string = "gauthier";
    std::cout << "string: " << toString( string ) << std::endl;
    std::cout << "string: " << string << std::endl;
    std::cout << std::endl;

    const Lambda lambda { 5, 1.0f, 2.0, { 1, 2, 3 }, "hello" };
    //    std::cout << "lambda: " << toString(lambda) << std::endl;
    std::cout << "lambda: " << lambda << std::endl;
    std::cout << std::endl;

    std::vector<std::string> strings{"gauthier", "bouyjou"};
    std::cout << "strings: " << toString( strings ) << std::endl;
    std::cout << "strings: " << strings << std::endl;
    std::cout << std::endl;

    TEST_END()
}
