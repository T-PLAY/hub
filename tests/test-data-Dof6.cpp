
#include <catch2/catch_test_macros.hpp>

#include <cmath>
#include <iostream>

#include <data/Dof6.hpp>

TEST_CASE( "Dof6 test" ) {

    unsigned char data[28] { 0 };
    const auto& resolution = hub::Resolution { { 1 }, hub::Format::DOF6 };
    hub::Measure measure( data, 28, resolution );
    const auto sqrt22 = sqrt( 2.0 ) / 2.0;

    hub::data::Dof6 dof6( measure );

    hub::data::Dof6 dof62( 0, 0, 0, 1, 0, 0, 0 );

    hub::data::Dof6 dof63( 1, 1, 1, 0, 0, 1, 0 );
    auto dof623 = hub::data::Dof6::slerp( dof62, dof63, 0.5 );
    CHECK( dof623 == hub::data::Dof6( 0.5, 0.5, 0.5, sqrt22, 0.0, sqrt22, 0.0 ) );

    hub::data::Dof6 dof65( 2, 2, 2, 0, 0, 0, 1 );
    auto dof625 = hub::data::Dof6::slerp( dof62, dof65, 0.5 );
    CHECK( dof625 == hub::data::Dof6( 1.0, 1.0, 1.0, sqrt22, 0.0, 0.0, sqrt22 ) );

    hub::data::Dof6 dof66( 2, 2, 2, 0, 0, 0, -1 );
    auto dof626 = hub::data::Dof6::slerp( dof62, dof66, 0.5 );
    CHECK( dof626 == hub::data::Dof6( 1.0, 1.0, 1.0, sqrt22, 0.0, 0.0, -sqrt22 ) );

    hub::data::Dof6 dof67( 2, 2, 2, 1, 0, 0, 0 );
    auto dof627 = hub::data::Dof6::slerp( dof62, dof67, 0.5 );
    CHECK( dof627 == hub::data::Dof6( 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0 ) );

    hub::data::Dof6 dof68( 2, 2, 2, -1, 0, 0, 0 );
    auto dof628 = hub::data::Dof6::slerp( dof62, dof68, 0.5 );
    CHECK( dof628 == hub::data::Dof6( 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0 ) );

    auto dof6Clone = dof6.clone();
    CHECK( dof6 == dof6Clone );

    hub::data::Dof6 dof64( 1, 2, 3, 0, 1, 0, 0 );
    CHECK( hub::data::Dof6::slerp( dof62, dof64, 0.0 ) == dof62 );
    CHECK( hub::data::Dof6::slerp( dof62, dof64, 1.0 ) == dof64 );

    const hub::Measure& measure2 = dof62;
    const hub::Measure& measure3 = dof63;
    CHECK( measure2.isInterpolable() );
    auto measure23 = hub::Measure::slerp( measure2, measure3, 0.5 );

    std::cout << dof6 << std::endl;
    std::cout << measure23 << std::endl;
}
