
#include "test_common.hpp"
//#include <catch2/catch_test_macros.hpp>
//#include <iostream>

#include <core/Any.hpp>
#include <map>

#include <data/Mat4.hpp>

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


    std::cout << "typeinfo(int) : " << typeid(int).name() << " : " << typeid(int).hash_code() << std::endl;
    std::cout << "typeinfo(double) : " << typeid(double).name() << " : " << typeid(int).hash_code() << std::endl;
    std::cout << "typeinfo(std::string) : " << typeid(std::string).name() << " : " << typeid(std::string).hash_code() << std::endl;
    std::cout  << std::endl;

    hub::Any any;
    assert(! any.hasValue());
//    std::cout << any << std::endl;

    any = 1;
    assert(any.hasValue());
//    std::cout << any.to_string<int>() << std::endl;
    std::cout << any << std::endl;
    CHECK( any.is<int>());
    CHECK( any.typeId() == typeid(int));
    CHECK( ! any.is<bool>());
    CHECK( ! any.is<double>());
    CHECK( ! any.is<const char *>());
    CHECK( ! any.is<std::string>());
    CHECK( ! any.is<Lambda>());
    int a = any.get<int>();
//    int a2 = any.get();
    CHECK( a == 1 );

    any = 1.0;
    std::cout << any << std::endl;
    CHECK (any.is<double>());
    CHECK (! any.is<int>());
    double b = any.get<double>();
//    auto b2 = any.get<int>();
    CHECK( b == 1.0 );

    any = std::string( "hello" );
    std::cout << any << std::endl;
    CHECK (any.is<std::string>());
    std::string c = any.get<std::string>();
    CHECK( c == "hello" );

    any = "hello";
    std::cout << any << std::endl;
    CHECK (any.is<const char *>());
    const char* d = any.get<const char*>();
    CHECK( !strcmp( d, "hello" ) );


    any = hub::data::Mat4();
    std::cout << any << std::endl;
    const auto& i = any.get<hub::data::Mat4>();
    CHECK( i == hub::data::Mat4() );


//    any = Lambda{5, 0.0, 1.0f, {0, 1, 2}};
//    CHECK (any.is<Lambda>());
//    std::cout << any << std::endl;

    std::cout << "any type: " << any.type() << std::endl;
    std::cout << "any value: " << any.getValueAsString() << std::endl;
    std::cout << "any typeid: " << any.typeId().name() << std::endl;
//    std::cout << any << std::endl;

}
