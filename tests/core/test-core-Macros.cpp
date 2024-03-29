

#include "test_common.hpp"
#include <iomanip>
#include <set>
#include <sstream>

//#include <core/Macros.hpp>
//#include <core/Traits.hpp>
#include <core/Base.hpp>

class Lambda
{
  public:
    int a    = 0;
    float b  = 0.0f;
    double c = 0.0;
    std::vector<int> ds;
    std::string e;
    auto toString() const { return hub::to_string( a, b, c, ds, e ); }
    static constexpr auto name() { return "Lambda"; };

    static constexpr auto Size = 5;
};

#if !defined( COMPILER_GCC ) && !defined( COMPILER_MSVC ) && !defined( COMPILER_CLANG )
#    error "compiler not supported"
#endif

#if !defined( OS_LINUX ) && !defined( OS_MACOS ) && !defined( OS_WINDOWS )
#    error "system not supported"
#endif

class Printer
{
  public:
    auto print() const {
        std::stringstream sstream;
        sstream << HEADER;
        return sstream.str();
    }
};

enum class UserEnum { A, B, C };

auto toString( UserEnum ue ) -> std::string {
    switch ( ue ) {
    case UserEnum::A:
        return "A";
        break;
    case UserEnum::B:
        return "B";
        break;
    case UserEnum::C:
        return "C";
        break;
    default:
        break;
    }
    return "None";
}

auto name( UserEnum ue ) -> std::string {
    return "UserEnum";
}

template <class T, std::size_t typeSize = 0, std::size_t typeId = 0>
void checkType( const std::string& typeName, const T& t ) {

    std::cout << std::setw( 30 ) << std::left;
    std::cout << typeName << ": ";
    std::cout << std::setw( 30 ) << std::left;
    std::cout << TYPE_NAME( t ) << " -> ";
    std::cout << std::setw( 50 ) << std::left;
    std::cout << t << std::endl;
    CHECK( TYPE_NAME( t ) == typeName );
    if constexpr ( typeSize != 0 ) { static_assert( hub::sizeOf<T>() == typeSize ); }
}

TEST_CASE( "Macros test" ) {
    TEST_BEGIN()

    CHECK( FILE_NAME == "test-core-Macros.cpp" );
    CHECK( FILE_NAME_WITHOUT_EXTENSION == "test-core-Macros" );

    auto printed = Printer().print();
    printed      = printed.substr( 5, printed.find_last_of( ":" ) - 4 );
    CHECK( printed == "[test-core-Macros:" );

    static_assert( hub::sizeOf<Lambda>() == 5 );
    static_assert( hub::sizeOf<int>() == 4 );
    static_assert( hub::sizeOf<int, int>() == 8 );
    static_assert( hub::sizeOf<Lambda, int>() == 9 );

    // C Types
    std::cout << "* C types :" << std::endl;
    static_assert( sizeof( int ) == 4 );
    checkType<int, 4, 12597418120160561823u>( "int", 5 );
    static_assert( sizeof( double ) == 8 );
    checkType<double, 8, 13841485242819600253u>( "double", 2.0 );
    checkType<float, 4>( "float", 3.0f );
    static_assert( sizeof( char ) == 1 );
    checkType<char, 1>( "char", 'c' );
    checkType<unsigned char, 1>( "uchar", 'd' );
#ifdef ARCH_X86
    checkType<const char*, 4>( "cstr", "bonjour" );
#else
    checkType<const char*, 8>( "cstr", "bonjour" );
#endif
    std::cout << std::endl;

    // C++ Types
    std::cout << "* C++ types :" << std::endl;
    checkType<std::string>( "string", "hello" );
    checkType<std::vector<int>>( "vector<int>", { 1, 2, 3 } );
    checkType<std::vector<unsigned char>>( "vector<uchar>", { 1, 2, 3 } );
    checkType<std::vector<const char*>>( "vector<cstr>", { "ab", "bc", "cd" } );
    checkType<std::vector<std::string>>( "vector<string>", { "a", "ab", "abc" } );
    checkType<std::vector<std::vector<int>>>( "vector<vector<int>>",
                                              { { 1 }, { 1, 2 }, { 1, 2, 3 } } );
    checkType<std::map<std::string, int>>( "map<string, int>", { { "a", 1 }, { "b", 2 } } );
    checkType<std::pair<std::string, int>>( "pair<string, int>", { "a", 1 } );
    checkType<std::tuple<int>>( "tuple<int>", 5 );
    checkType<std::tuple<int, double, bool, float, char, std::string>>(
        "tuple<int, double, bool, float, char, string>", { 5, 2.0, true, 3.0f, 'a', "hello" } );
    checkType<std::set<std::string>>( "set<string>", { "abc", "def" } );
    std::cout << std::endl;

    // User Types
    struct UserClass {
        int a;
        double b;
        bool c;
        static constexpr auto name() { return "UserClass"; }
        auto toString() const { return hub::to_string( a, b, c ); }
    };

    std::cout << "* User types :" << std::endl;
    checkType<UserClass, 24>( "UserClass", { 1, 2.0, false } );
    checkType<Lambda, 5>( "Lambda", Lambda { 1, 2.0f, 3.0, { 1, 2 }, "abc" } );
    checkType<UserEnum>( "UserEnum", UserEnum::A );
    std::cout << std::endl;

    TEST_END()
}
