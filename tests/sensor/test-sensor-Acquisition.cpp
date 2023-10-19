
#include <catch2/catch_test_macros.hpp>

#include <sensor/Acquisition.hpp>

TEST_CASE( "Acquisition test" ) {

    const hub::sensor::Resolutions resolutions { { hub::sensor::format::BGR8, 1 } };

    // getSize
    {
        //        hub::sensor::Acquisition acq { 0, 0 };
        hub::sensor::Acquisition acq { resolutions };
        //        hub::sensor::Acquisition acq();
        CHECK( acq.nByte() == 2 * sizeof(long long) + 3 );
        unsigned char data[3] { 1, 2, 3 };
        //        acq << hub::Measure { data, 3, hub::sensor::Resolution { { 1 },
        //        hub::sensor::Format::BGR8 } };
        auto& measure = acq.getMeasures().at( 0 );
        measure.setData( data, 3 );
        CHECK( measure.nByte() == 3 );

        //        acq.emplaceMeasure( data, 3, hub::sensor::Resolution { { 1 },
        //        hub::sensor::Format::BGR8 } ); CHECK( acq.getSize() == 6 );

        //        acq.pushBack(
        //            hub::Measure { data, 3, hub::sensor::Resolution { { 1 },
        //            hub::sensor::Format::BGR8 } } );
        //        CHECK( acq.getSize() == 9 );
    }

    //    hub::sensor::Acquisition acq( 0, 1 );
    hub::sensor::Acquisition acq { resolutions };
    acq.setStart( 0 );
    acq.setEnd( 1 );

    hub::sensor::Acquisition acq2( std::move( acq ) );

    CHECK( acq2.getStart() == 0 );
    CHECK( acq2.getEnd() == 1 );

    hub::sensor::Resolution resolution { hub::sensor::format::RGB8, 1 };
    unsigned char data[3] { 0, 1, 2 };

//    hub::sensor::Measure measure( data, 3, resolution );
//    hub::sensor::Measure measure{resolution, data};
    auto& measure = acq2.getMeasures().at( 0 );
    measure.setData(data, 3);
//    acq2.emplaceMeasure( data, 3, resolution );

    const auto& measures = acq2.getMeasures();
    CHECK( measures.size() == 1 );

//    hub::sensor::Acquisition acq3( 0, 1 );
//    acq3 << std::move( measure );

//    CHECK( acq2 == acq3 );
//    CHECK( acq2 == hub::sensor::Acquisition( 0, 1 ) << hub::Measure( data, 3, resolution ) );

//    unsigned char data2[3] { 0, 1, 3 };
//    CHECK( acq2 != hub::sensor::Acquisition( 0, 1 ) << hub::Measure( data2, 3, resolution ) );

//    CHECK( !acq2.isInterpolable() );

//    for ( int i = 1; i < (int)hub::sensor::Format::COUNT; ++i ) {
//        const auto& format             = hub::sensor::Format( i );
//        const auto& resolution2        = hub::sensor::Resolution( { 1 }, format );
//        hub::sensor::Acquisition acq22 = std::move(
//            hub::sensor::Acquisition( 0, 0 )
//            << hub::Measure( data,
//                             hub::sensor::resolution::computeAcquisitionSize( resolution2 ),
//                             resolution2 ) );
//        CHECK( acq22.isInterpolable() ==
//               hub::sensor::resolution::s_format2isInterpolable[(int)format] );
//    }

//    hub::sensor::Acquisition acq4 =
//        std::move( hub::sensor::Acquisition( 0, 0 ) << hub::data::Dof6( 0, 0, 0, 1, 0, 0, 0 ) );
//    hub::sensor::Acquisition acq5 =
//        std::move( hub::sensor::Acquisition( 2, 2 ) << hub::data::Dof6( 1, 1, 1, 1, 0, 0, 0 ) );
//    std::cout << acq4 << std::endl;
//    std::cout << acq5 << std::endl;

//    hub::sensor::Acquisition acq45 = hub::sensor::Acquisition::slerp( acq4, acq5, 0.5 );
//    std::cout << acq45 << std::endl;
//    CHECK( acq45 ==
//           ( hub::sensor::Acquisition( 1, 1 ) << hub::data::Dof6( 0.5, 0.5, 0.5, 1, 0, 0, 0 ) ) );

//    hub::sensor::Acquisition acq6( acq2.getStart(), acq2.getEnd() );
//    acq6 << measures;
//    CHECK( acq6 == acq2 );

//    hub::sensor::Acquisition acq7 = acq6.clone();
//    CHECK( acq6 == acq7 );

//    CHECK( acq6.getSize() == acq7.getSize() );

//    CHECK( !( acq7 == acq4 ) );

//    // hasFixedSize
//    {
//        hub::sensor::Acquisition acq8 { 0, 0 };
//        CHECK( acq8.hasFixedSize() );

//        unsigned char data3[3] { 1, 2, 3 };
//        acq8 << hub::Measure {
//            data3, 3, hub::sensor::Resolution { { 1 }, hub::sensor::Format::BGR8 } };
//        CHECK( acq8.hasFixedSize() );

//        acq8.emplaceMeasure(
//            data3, 3, hub::sensor::Resolution { { 1 }, hub::sensor::Format::BGR8 } );
//        CHECK( acq8.hasFixedSize() );

//        acq8.pushBack( hub::Measure {
//            data3, 3, hub::sensor::Resolution { { 1 }, hub::sensor::Format::USER_DATA } } );
//        CHECK( !acq8.hasFixedSize() );
//    }
}
