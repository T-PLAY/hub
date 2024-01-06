
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT

#include "test_common.hpp"

#include <array>
#include <core/Traits.hpp>
#include <span>

TEST_CASE( "Traits test" ) {
    TEST_BEGIN()

    // C Types
    std::cout << "* C types :" << std::endl;

    const int a = 5;
    std::cout << "int: " << std::to_string( a ) << std::endl;
    std::cout << "int: '" << TYPE_NAME( a ) << "' -> " << hub::to_string( a ) << std::endl;
    assert( TYPE_NAME( a ) == "int" );
    std::cout << a << std::endl;

    const double d = 5;
    std::cout << "double: " << std::to_string( d ) << std::endl;
    std::cout << "double: '" << TYPE_NAME( d ) << "' -> " << hub::to_string( d ) << std::endl;
    assert( TYPE_NAME( d ) == "double" );
    std::cout << d << std::endl;

    const float f = 5;
    std::cout << "float: " << std::to_string( f ) << std::endl;
    std::cout << "float: '" << TYPE_NAME( f ) << "' -> " << hub::to_string( f ) << std::endl;
    assert( TYPE_NAME( f ) == "float" );
    std::cout << f << std::endl;

    const char c = 'c';
    std::cout << "char: " << std::to_string( c ) << std::endl;
    std::cout << "char: '" << TYPE_NAME( c ) << "' -> " << hub::to_string( c ) << std::endl;
    assert( TYPE_NAME( c ) == "char" );
    std::cout << c << std::endl;

    const unsigned char uc = 'd';
    std::cout << "uchar: " << std::to_string( uc ) << std::endl;
    std::cout << "uchar: '" << TYPE_NAME( uc ) << "' -> " << hub::to_string( uc ) << std::endl;
    assert( TYPE_NAME( uc ) == "unsigned char" );
    std::cout << uc << std::endl;

    const char* cstr = "bonjour";
    std::cout << "cstr: " << cstr << std::endl;
    std::cout << "cstr: '" << TYPE_NAME( cstr ) << "' -> " << hub::to_string( cstr ) << std::endl;
    assert( TYPE_NAME( cstr ) == "char const*" );
    std::cout << cstr << std::endl;

    const unsigned char* ucstr = (unsigned char*)"hello";
    std::cout << "ucstr: " << ucstr << std::endl;
    std::cout << "ucstr: '" << TYPE_NAME( ucstr ) << "' -> " << hub::to_string( ucstr )
              << std::endl;
    assert( TYPE_NAME( ucstr ) == "unsigned char const*" );
    std::cout << ucstr << std::endl;

    std::cout << std::endl;

    // C++ Types
    std::cout << "* C++ types :" << std::endl;

    const std::string string = "gauthier";
    std::cout << "string: " << hub::to_string( string ) << std::endl;
    std::cout << "string: " << string << std::endl;
    std::cout << std::endl;


    const std::string str = "hello";
    std::cout << "str: " << str << std::endl;
    std::cout << "str: '" << TYPE_NAME( str ) << "' -> " << hub::to_string( str ) << std::endl;
    std::cout << str << std::endl;

    const std::array<int, 5> array { 1, 2, 3, 4, 5 };
    std::cout << "array: " << hub::to_string( array ) << std::endl;
    std::cout << "array: " << array << std::endl;
    std::cout << std::endl;

#if CPP_VERSION >= 20
    const std::span span { array };
    std::cout << "span: " << hub::to_string( span ) << std::endl;
    std::cout << "span: " << span << std::endl;
    std::cout << std::endl;
#endif


    // vector
    const std::vector<int> vints { 1, 2, 3 };
    std::cout << "vints: " << hub::to_string( vints ) << std::endl;
    std::cout << "vints: " << vints << std::endl;
    std::cout << std::endl;

    const std::vector<char> vchar { 'a', 'b', 'c' };
    std::cout << "vchar: " << hub::to_string( vchar ) << std::endl;
    std::cout << "vchar: " << vchar << std::endl;
    std::cout << std::endl;

    std::vector<std::string> strings { "gauthier", "bouyjou" };
    std::cout << "strings: " << hub::to_string( strings ) << std::endl;
    std::cout << "strings: " << strings << std::endl;
    std::cout << std::endl;

    // std::vector<std::string> strings { "a", "b", "c" };
    // std::cout << "strings: '" << TYPE_NAME( strings ) << "' -> " << hub::to_string( strings )
    //           << std::endl;
    // std::cout << strings << std::endl;

    std::vector<int> ints { 1, 2, 3 };
    std::cout << "ints: '" << TYPE_NAME( ints ) << "' -> " << hub::to_string( ints ) << std::endl;
    std::cout << ints << std::endl;

    std::vector<unsigned char> uchars { 'a', 'b', 'c' };
    std::cout << "uchars: '" << TYPE_NAME( uchars ) << "' -> " << hub::to_string( uchars )
              << std::endl;
    std::cout << uchars << std::endl;

    std::vector<const char*> cstrings { "a", "b", "c" };
    std::cout << "cstrings: '" << TYPE_NAME( cstrings ) << "' -> " << hub::to_string( cstrings )
              << std::endl;
    std::cout << cstrings << std::endl;


    std::vector<std::vector<int>> intss { { 1 }, { 1, 2 }, { 1, 2, 3 } };
    std::cout << "intss: '" << TYPE_NAME( intss ) << "' -> " << hub::to_string( intss )
              << std::endl;
    std::cout << intss << std::endl;

    std::map<std::string, int> map { { "a", 1 }, { "b", 2 } };
    std::cout << "map: '" << TYPE_NAME( map ) << "' -> " << hub::to_string( map ) << std::endl;
    std::cout << map << std::endl;

    std::pair<std::string, int> pair { "a", 1 };
    std::cout << "pair: '" << TYPE_NAME( pair ) << "' -> " << hub::to_string( pair ) << std::endl;
    std::cout << hub::typeName( pair ) << std::endl;
    std::cout << pair << std::endl;

    std::tuple<int> tuple { 5 };
    static_assert( std::tuple_size<decltype( tuple )>() == 1 );
    static_assert( std::is_same_v<int, std::remove_cvref_t<decltype( std::get<0>( tuple ) )>> );
    std::cout << "tuple: '" << TYPE_NAME( tuple ) << "' -> " << hub::to_string( tuple )
              << std::endl;
    std::cout << tuple << std::endl;

    std::tuple<int, double, bool, float, char, std::string> tuple3 {
        5, 2.0, true, 3.0f, 'a', "hello" };
    std::cout << "tuple3: '" << TYPE_NAME( tuple3 ) << "' -> " << hub::to_string( tuple3 )
              << std::endl;
    std::cout << tuple3 << std::endl;

    struct UserClass {
        int a;
        double b;
        bool c;
        static constexpr auto name() { return "UserClass"; }
        auto toString() const {
            return std::to_string( a ) + " " + std::to_string( b ) + " " + hub::to_string( c );
        }
    };

    UserClass userClass { 1, 2.0, false };
    // std::cout << "userClass: " << TYPE_NAME( userClass ) << std::endl;
    std::cout << "userClass: '" << TYPE_NAME( userClass ) << "' -> " << hub::to_string( userClass )
              << std::endl;
    assert( TYPE_NAME( userClass ) == "UserClass" );
    std::cout << userClass << std::endl;

    //    using namespace hub;

    class Lambda
    {
      public:
        int a    = 0;
        float b  = 0.0f;
        double c = 0.0;
        std::vector<int> ds;
        std::string e;

        // friend std::ostream& operator<<( std::ostream& os, const Lambda& lambda ) {
        //     os << lambda.a << " " << lambda.b << " " << lambda.c << " " << lambda.ds << " "
        //        << lambda.e;
        //     return os;
        // }
    };

    const Lambda lambda { 5, 1.0f, 2.0, { 1, 2, 3 }, "hello" };
    //    std::cout << "lambda: " << to_string(lambda) << std::endl;
    // std::cout << "lambda: " << lambda << std::endl;
    // std::cout << std::endl;


    TEST_END()
}
