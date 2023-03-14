
#include <catch2/catch_test_macros.hpp>

#include <cmath>
#include <iostream>

#include <data/Dof6.hpp>
#include <data/Measure.hpp>

TEST_CASE( "Measure test" ) {

    unsigned char data[28] { 0 };
    const auto& resolution = hub::Resolution { { 1 }, hub::Format::RGB8 };
    hub::data::Measure measure( data, 3, resolution );

    hub::data::Measure measure2( reinterpret_cast<const unsigned char*>( data ), 3, resolution );
    CHECK( measure == measure2 );

    hub::data::Measure measure3( measure.clone() );
    CHECK( measure == measure3 );
    CHECK( !measure3.isInterpolable() );
    std::cout << measure3 << std::endl;

    hub::data::Measure measure4( std::move( measure3 ) );
    CHECK( measure == measure4 );

    unsigned char data2[56] { 1 };
    const auto& resolution2 = hub::Resolution { { 2 }, hub::Format::DOF6 };
    hub::data::Measure measure5( data2, 56, resolution2 );
    CHECK( measure != measure5 );

    unsigned char data3[3] { 1 };
    hub::data::Measure measure6( data3, 3, resolution );

    CHECK( hub::data::Measure::slerp( measure, measure6, 0.5 ) == measure );

    const auto sqrt22 = sqrt( 2.0 ) / 2.0;

    hub::data::Dof6 dof62( 0, 0, 0, 1, 0, 0, 0 );

    hub::data::Dof6 dof63( 1, 1, 1, 0, 0, 1, 0 );
    auto dof623 = hub::data::Measure::slerp( dof62, dof63, 0.5 );
    CHECK( dof623 == hub::data::Dof6( 0.5, 0.5, 0.5, sqrt22, 0.0, sqrt22, 0.0 ) );
}
