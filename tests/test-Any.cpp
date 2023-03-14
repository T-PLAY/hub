
#include <catch2/catch_test_macros.hpp>

#include <iostream>

#include <Any.hpp>

#include <data/Mat4.hpp>
//#include <string>
//#include <iostream>
//#include <cstring>

TEST_CASE( "Any test" ) {

    hub::Any any;
    std::cout << any << std::endl;

    any = 1;
    std::cout << any << std::endl;
    int a = any.getInt();
    CHECK( a == 1 );

    any = 1.0;
    std::cout << any << std::endl;
    double b = any.getDouble();
    CHECK( b == 1.0 );

    any = std::string( "hello" );
    std::cout << any << std::endl;
    std::string c = any.getStdString();
    CHECK( c == "hello" );

    any = "hello";
    std::cout << any << std::endl;
    const char* d = any.getConstCharPtr();
    CHECK( !strcmp( d, "hello" ) );

    any = hub::data::Mat4();
    std::cout << any << std::endl;
    const auto& i = any.getMat4();
    CHECK( i == hub::data::Mat4() );

    std::cout << any.type() << std::endl;
}
