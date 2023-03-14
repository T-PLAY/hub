
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

    //    any = std::vector<float>{1.0f};
    //    std::cout << any << std::endl;
    //    std::vector<float> e = any.getStdVectorFloat();
    //    CHECK(e.size() == 1);
    //    CHECK(e.front() == 1.0f);

    //    any = 1u;
    //    std::cout << any << std::endl;
    //    uint f = any.getUnsignedInt();
    //    CHECK(f == 1u);

    ////    auto v = {0.0f};
    //    const float floats[3] {0.0f, 1.0f, 2.0f};
    //    any = floats;
    //    std::cout << any << std::endl;
    //    const float * g = any.getConstFloatPtr();
    ////    std::cout << "g: " << g << std::endl;
    ////    std::cout << "floats: " << floats << std::endl;
    //    CHECK(g == floats);
    //    CHECK(g[0] == 0.0f);
    //    CHECK(g[1] == 1.0f);
    //    CHECK(g[2] == 2.0f);

    //    any = {0.0};
    //    const double doubles[3] {0.0, 1.0, 2.0};
    //    any = doubles;
    //    std::cout << any << std::endl;
    //    const double * h = any.getConstDoublePtr();
    //    CHECK(h == doubles);
    //    CHECK(h[0] == 0.0);
    //    CHECK(h[1] == 1.0);
    //    CHECK(h[2] == 2.0);
}
