
#include <catch2/catch_test_macros.hpp>

#include <iostream>

#include <data/Mat4.hpp>

TEST_CASE( "Mat4 test" ) {

    //    float matData[16] = { 0 };
    //    hub::data::Mat4 mat4( matData );
    hub::data::Mat4 mat4;

    float data[16] { 0.0 };
    hub::data::Mat4 mat42 { data };

    hub::data::Mat4 mat43( mat42 );
    CHECK( mat43 == mat42 );

    std::cout << mat43 << std::endl;

    //    CHECK( mat4 != measure2 );
    //    CHECK(measure23 == dof623);
    //    std::cout << mat4 << std::endl;
}
