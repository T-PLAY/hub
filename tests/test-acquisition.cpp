
#include <catch2/catch_test_macros.hpp>

#include <Acquisition.hpp>

TEST_CASE( "Acquisition test" ) {

    // getSize
    {
        hub::Acquisition acq { 0, 0 };
        CHECK( acq.getSize() == 0 );
        unsigned char data[3] { 1, 2, 3 };
        acq << hub::data::Measure { data, 3, hub::Resolution { { 1 }, hub::Format::BGR8 } };
        CHECK( acq.getSize() == 3 );

        acq.emplaceMeasure( data, 3, hub::Resolution { { 1 }, hub::Format::BGR8 } );
        CHECK( acq.getSize() == 6 );

        acq.pushBack( hub::data::Measure { data, 3, hub::Resolution { { 1 }, hub::Format::BGR8 } } );
        CHECK( acq.getSize() == 9 );

        //        acq << hub::Measure{ data, 3, hub::Resolution{{1}, hub::Format::USER_DATA}};
        //        CHECK(acq.getSize() == 12);
    }

    hub::Acquisition acq( 0, 1 );

    hub::Acquisition acq2( std::move( acq ) );

    CHECK( acq2.m_start == 0 );
    CHECK( acq2.m_end == 1 );

    //    hub::Acquisition acq3(std::move(hub::Acquisition(0, 1)));

    //    const hub::Resolution resolution({1}, hub::Format::RGB8);
    hub::Resolution resolution( { 1 }, hub::Format::RGB8 );
    unsigned char data[3] { 0, 1, 2 };
    //    acq2.emplaceMeasure(std::move(measure));

    hub::data::Measure measure( data, 3, resolution );

    acq2.emplaceMeasure( data, 3, resolution );

    //    CHECK(acq2.getMeasures())
    const auto& measures = acq2.getMeasures();
    CHECK( measures.size() == 1 );

    hub::Acquisition acq3( 0, 1 );
    acq3 << std::move( measure );

    CHECK( acq2 == acq3 );
    CHECK( acq2 == hub::Acquisition( 0, 1 ) << hub::data::Measure( data, 3, resolution ) );

    unsigned char data2[3] { 0, 1, 3 };
    CHECK( acq2 != hub::Acquisition( 0, 1 ) << hub::data::Measure( data2, 3, resolution ) );

    CHECK( !acq2.isInterpolable() );

    //    hub::Acquisition acq4 = std::move(hub::Acquisition(0, 0) << hub::Measure(data2, 3,
    //    resolution)); CHECK ( ! acq4.isInterpolable());

    //    hub::s_format2isInterpolable
    for ( int i = 1; i < (int)hub::Format::COUNT; ++i ) {
        const auto& format     = hub::Format( i );
        const auto& resolution = hub::Resolution( { 1 }, format );
        hub::Acquisition acq   = std::move(
            hub::Acquisition( 0, 0 )
            << hub::data::Measure( data, hub::computeAcquisitionSize( resolution ), resolution ) );
        CHECK( acq.isInterpolable() == hub::s_format2isInterpolable[(int)format] );
    }

    const hub::Resolution resolution2( { 1 }, hub::Format::DOF6 );
    //    hub::Dof6 dof6(0, 0, 0, 1, 0, 0, 0);

    hub::Acquisition acq4 =
        std::move( hub::Acquisition( 0, 0 ) << hub::data::Dof6( 0, 0, 0, 1, 0, 0, 0 ) );
    hub::Acquisition acq5 =
        std::move( hub::Acquisition( 2, 2 ) << hub::data::Dof6( 1, 1, 1, 1, 0, 0, 0 ) );
    std::cout << acq4 << std::endl;
    std::cout << acq5 << std::endl;

    hub::Acquisition acq45 = hub::Acquisition::slerp( acq4, acq5, 0.5 );
    std::cout << acq45 << std::endl;
    CHECK( acq45 == ( hub::Acquisition( 1, 1 ) << hub::data::Dof6( 0.5, 0.5, 0.5, 1, 0, 0, 0 ) ) );

    //    const hub::Measure measure2(data, 3, resolution);
    hub::Acquisition acq6( acq2.m_start, acq2.m_end );
    acq6 << measures;
    //    CHECK(measures.size() == 2);
    CHECK( acq6 == acq2 );

    hub::Acquisition acq7 = acq6.clone();
    CHECK( acq6 == acq7 );

    CHECK( acq6.getSize() == acq7.getSize() );

    CHECK( !( acq7 == acq4 ) );

    // hasFixedSize
    {
        hub::Acquisition acq { 0, 0 };
        CHECK( acq.hasFixedSize() );

        unsigned char data[3] { 1, 2, 3 };
        acq << hub::data::Measure { data, 3, hub::Resolution { { 1 }, hub::Format::BGR8 } };
        CHECK( acq.hasFixedSize() );

        acq.emplaceMeasure( data, 3, hub::Resolution { { 1 }, hub::Format::BGR8 } );
        CHECK( acq.hasFixedSize() );

        acq.pushBack( hub::data::Measure { data, 3, hub::Resolution { { 1 }, hub::Format::USER_DATA } } );
        CHECK( !acq.hasFixedSize() );

        //        acq << hub::Measure{ data, 3, hub::Resolution{{1}, hub::Format::USER_DATA}};
        //        CHECK(acq.getSize() == 12);
    }
}
