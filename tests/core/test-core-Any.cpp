
#include "test_common.hpp"
//#include <catch2/catch_test_macros.hpp>
//#include <iostream>

#include <core/Any.hpp>
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
    bool operator==(const Lambda & lambda) const {
        return a == lambda.a && b == lambda.b && c == lambda.c && ds == lambda.ds;
    }
};


TEST_CASE( "Any test" ) {
//int main() {

    hub::Any any;
    hub::Any any2;
    assert(! any.hasValue());
    assert(any.type() == typeid(void));
//    std::cout << any.type().name() << std::endl;
    std::cout << any << std::endl;

    any = 1;
    any2 = 1;
    CHECK(any == any2);
    any2 = 2;
    CHECK(any != any2);
    std::cout << any << std::endl;
    std::cout << any.type().name() << std::endl;
    std::cout << any.getValueStr() << std::endl;
    assert(any.hasValue());
//    std::cout << any.to_string<int>() << std::endl;
    CHECK( any.is<int>());
//    CHECK( any.typeId() == typeid(int));
    CHECK( ! any.is<bool>());
    CHECK( ! any.is<double>());
    CHECK( ! any.is<const char *>());
    CHECK( ! any.is<std::string>());
    CHECK( ! any.is<Lambda>());
    int a = any.get<int>();
//    int a2 = any.get();
    CHECK( a == 1 );

    any = 1.4;
    any2 = 1.4;
    CHECK(any == any2);
    any2 = 2.1;
    CHECK(any != any2);
    std::cout << any << std::endl;
    CHECK (any.is<double>());
    CHECK (! any.is<int>());
    double b = any.get<double>();
//    auto b2 = any.get<int>();
    CHECK( b == 1.4 );

    any = std::string( "hello" );
    any2 = std::string("hello");
    CHECK(any == any2);
    any2 = std::string("bye");
    CHECK(any != any2);
    std::cout << any << std::endl;
    CHECK (any.is<std::string>());
    std::string c = any.get<std::string>();
    CHECK( c == "hello" );

    const char * str = "hello";
    any = str;
    any2 = str;
    CHECK(any == any2);
    any2 = "bye";
    CHECK(any != any2);
    std::cout << any << std::endl;
    CHECK (any.is<const char *>());
    const char* d = any.get<const char*>();
    CHECK( !strcmp( d, "hello" ) );



//    any = hub::data::Mat4();
//    std::cout << any << std::endl;
//    const auto& i = any.get<hub::data::Mat4>();
//    CHECK( i == hub::data::Mat4() );


    const Lambda lambda{5, 0.0, 1.0f, {0, 1, 2}};
    any = lambda;
    any2 = lambda;
    CHECK(any == any2);
    any2 = Lambda{2, 0.0, 1.0f, {0}};
    CHECK(any != any2);
    std::cout << any << std::endl;
    CHECK (any.is<Lambda>());
    CHECK(any.get<Lambda>() == lambda);

//    std::cout << "any type: " << any.type() << std::endl;
//    std::cout << "any value: " << any.getValueAsString() << std::endl;
//    std::cout << "any typeid: " << any.typeId().name() << std::endl;
//    std::cout << any << std::endl;

}
