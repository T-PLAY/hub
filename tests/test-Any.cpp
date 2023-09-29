
#include "test_common.hpp"
//#include <catch2/catch_test_macros.hpp>
//#include <iostream>

#include <Any.hpp>
#include <map>

//#include <data/Mat4.hpp>

class Lambda {
  public:
    int a;
    float b;
    double c;
    std::vector<int> ds;
    friend std::ostream& operator<<( std::ostream& os, const Lambda& lambda ) {
        os << lambda.a << lambda.b << lambda.c;
        for (const auto & d : lambda.ds) {
            os << d;
        }
        return os;
    }
};


TEST_CASE( "Any test" ) {

    hub::Any any;
    assert(any.isEmpty());
//    std::cout << any << std::endl;

    any = 1;
    assert(! any.isEmpty());
    std::cout << any.to_string<int>() << std::endl;
    CHECK( ! any.is<std::string>());
    CHECK( any.is<int>());
    int a = any.get<int>();
    CHECK( a == 1 );

    any = 1.0;
    std::cout << any.to_string<double>() << std::endl;
    double b = any.get<double>();
    CHECK( b == 1.0 );

    any = std::string( "hello" );
    std::cout << any.to_string<std::string>() << std::endl;
    std::string c = any.get<std::string>();
    CHECK( c == "hello" );

    any = "hello";
    std::cout << any.to_string<const char*>() << std::endl;
    const char* d = any.get<const char*>();
    CHECK( !strcmp( d, "hello" ) );

    any = Lambda{5, 0.0, 1.0f, {0, 1, 2}};
    std::cout << any.to_string<Lambda>() << std::endl;

//    any = hub::data::Mat4();
//    std::cout << any << std::endl;
//    const auto& i = any.getMat4();
//    CHECK( i == hub::data::Mat4() );
//    std::cout << any << std::endl;

//    std::cout << any.value_to_string() << std::endl;
//    std::cout << any << std::endl;

//    std::cout << any.type() << std::endl;
}
