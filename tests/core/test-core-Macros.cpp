
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

    static_assert(hub::sizeOf<int>() == 4);
    static_assert(hub::sizeOf<double>() == 8);
    static_assert(hub::sizeOf<int, int>() == 8);
    static_assert(hub::sizeOf<Lambda>() == 5);

    std::cout << "int: " << TYPE_NAME(int) << std::endl;
    std::cout << "vints: " << TYPE_NAME(std::vector<int>) << std::endl;

    std::cout << "string: " << TYPE_NAME(std::string) << std::endl;
    std::cout << "strings: " << TYPE_NAME(std::vector<std::string>) << std::endl;

    std::cout << "lambda: " << TYPE_NAME(Lambda) << std::endl;
    std::cout << "lambdas: " << TYPE_NAME(std::vector<Lambda>) << std::endl;

    // C Types
    std::cout << "* C types :" << std::endl;

    const int a = 5;
    std::cout << "int: " << std::to_string( a ) << std::endl;
    std::cout << "int: '" << TYPE_NAME( a ) << "' -> " << hub::to_string( a ) << std::endl;
    assert( TYPE_NAME( a ) == "int" );

    const double d = 5;
    std::cout << "double: " << std::to_string( d ) << std::endl;
    std::cout << "double: '" << TYPE_NAME( d ) << "' -> " << hub::to_string( d ) << std::endl;
    assert( TYPE_NAME( d ) == "double" );

    const float f = 5;
    std::cout << "float: " << std::to_string( f ) << std::endl;
    std::cout << "float: '" << TYPE_NAME( f ) << "' -> " << hub::to_string( f ) << std::endl;
    assert( TYPE_NAME( f ) == "float" );

    const char c = 'c';
    std::cout << "char: " << std::to_string( c ) << std::endl;
    std::cout << "char: '" << TYPE_NAME( c ) << "' -> " << hub::to_string( c ) << std::endl;
    assert( TYPE_NAME( c ) == "char" );

    const unsigned char uc = 'd';
    std::cout << "uchar: " << std::to_string( uc ) << std::endl;
    std::cout << "uchar: '" << TYPE_NAME( uc ) << "' -> " << hub::to_string( uc ) << std::endl;
    assert( TYPE_NAME( uc ) == "unsigned char" );

    const char* cstr = "bonjour";
    std::cout << "cstr: " << cstr << std::endl;
    std::cout << "cstr: '" << TYPE_NAME( cstr ) << "' -> " << hub::to_string( cstr ) << std::endl;
    assert( TYPE_NAME( cstr ) == "char const*" );

    const unsigned char* ucstr = (unsigned char*)"hello";
    std::cout << "ucstr: " << ucstr << std::endl;
    std::cout << "ucstr: '" << TYPE_NAME( ucstr ) << "' -> " << hub::to_string( ucstr )
              << std::endl;
    assert( TYPE_NAME( ucstr ) == "unsigned char const*" );

    std::cout << std::endl;

    // C++ Types
    std::cout << "* C++ types :" << std::endl;

    const std::string str = "hello";
    std::cout << "str: " << str << std::endl;
    std::cout << "str: '" << TYPE_NAME( str ) << "' -> " << hub::to_string( str ) << std::endl;

    std::vector<int> ints { 1, 2, 3 };
    std::cout << "ints: '" << TYPE_NAME( ints ) << "' -> " << hub::to_string( ints ) << std::endl;

    std::vector<unsigned char> uchars { 'a', 'b', 'c' };
    std::cout << "uchars: '" << TYPE_NAME( uchars ) << "' -> " << hub::to_string( uchars )
              << std::endl;

    std::vector<const char*> cstrings { "a", "b", "c" };
    std::cout << "cstrings: '" << TYPE_NAME( cstrings ) << "' -> " << hub::to_string( cstrings )
              << std::endl;

    std::vector<std::string> strings { "a", "b", "c" };
    std::cout << "strings: '" << TYPE_NAME( strings ) << "' -> " << hub::to_string( strings )
              << std::endl;

    std::vector<std::vector<int>> intss { { 1 }, { 1, 2 }, { 1, 2, 3 } };
    std::cout << "intss: '" << TYPE_NAME( intss ) << "' -> " << hub::to_string( intss )
              << std::endl;

    std::map<std::string, int> map { { "a", 1 }, { "b", 2 } };
    std::cout << "map: '" << TYPE_NAME( map ) << "' -> " << hub::to_string( map ) << std::endl;

    std::pair<std::string, int> pair { "a", 1 };
    std::cout << "pair: '" << TYPE_NAME( pair ) << "' -> " << hub::to_string( pair ) << std::endl;
    std::cout << hub::typeName( pair ) << std::endl;

    std::tuple<int> tuple { 5 };
    static_assert( std::tuple_size<decltype( tuple )>() == 1 );
    static_assert( std::is_same_v<int, std::remove_cvref_t<decltype( std::get<0>( tuple ) )>> );
    std::cout << "tuple: '" << TYPE_NAME( tuple ) << "' -> " << hub::to_string( tuple )
              << std::endl;

    std::tuple<int, double, bool, float, char, std::string> tuple3 {
        5, 2.0, true, 3.0f, 'a', "hello" };
    std::cout << "tuple3: '" << TYPE_NAME( tuple3 ) << "' -> " << hub::to_string( tuple3 )
              << std::endl;

    struct UserClass {
        int a;
        double b;
        bool c;
        static constexpr auto name() {
            return "UserClass";
        }
        auto toString() const {
            return std::to_string(a) + " " + std::to_string(b) + " " + hub::to_string(c);
        }
    };

    UserClass userClass { 1, 2.0, false };
    // std::cout << "userClass: " << TYPE_NAME( userClass ) << std::endl;
    std::cout << "userClass: '" << TYPE_NAME( userClass ) << "' -> " << hub::to_string( userClass )
              << std::endl;
    assert( TYPE_NAME( userClass ) == "UserClass" );


    TEST_END()
}
