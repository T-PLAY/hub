
#include <catch2/catch_test_macros.hpp>

#include <io/File.hpp>
#include <io/Ram.hpp>
#include <InputSensor.hpp>
#include <OutputSensor.hpp>

TEST_CASE( "Measures test" ) {

    unsigned char data[28] { 0 };
    const auto& resolution = hub::Resolution { { 1 }, hub::Format::DOF6 };
    hub::data::Measure measure( data, 28, resolution );

    hub::data::Dof6 dof6( measure );

    hub::data::Dof6 dof62( 0, 0, 0, 1, 0, 0, 0 );
    hub::data::Dof6 dof63( 1, 1, 1, 1, 0, 0, 0 );
    auto dof623 = hub::data::Dof6::slerp( dof62, dof63, 0.5 );
    CHECK( dof623 == hub::data::Dof6( 0.5, 0.5, 0.5, 1, 0, 0, 0 ) );

    auto dof6Clone = dof6.clone();
    CHECK( dof6 == dof6Clone );

    hub::data::Dof6 dof64( 1, 2, 3, -1, 0, 0, 0 );
    //    auto dof624 = hub::Dof6::slerp(dof62, dof64, 1.0);
    CHECK( hub::data::Dof6::slerp( dof62, dof64, 0.0 ) == dof62 );
    //    CHECK(hub::Dof6::slerp(dof62, dof64, 1.0)  == dof64);
    //    CHECK(dof624 == dof64);

    hub::data::Measure& measure2 = dof62;
    hub::data::Measure& measure3 = dof63;
    CHECK( measure2.isInterpolable() );
    auto measure23 = hub::data::Measure::slerp( measure2, measure3, 0.5 );

    std::cout << dof6 << std::endl;

//    float matData[16] = { 0 };
//    hub::data::Mat4 mat4( matData );
    hub::data::Mat4 mat4;
    CHECK( mat4 != measure2 );
    //    CHECK(measure23 == dof623);
    std::cout << mat4 << std::endl;

    hub::data::Measure measure4 = std::move( measure );

    CHECK( measure4 != measure2 );
    std::cout << measure4 << std::endl;

    hub::data::UserData userData( "userData", 0 );
    std::cout << userData << std::endl;

    hub::data::Measure& measure5 = userData;
    hub::data::UserData userData2( measure5 );
    //    hub::Measure & measure6 = userData;
    //    CHECK()

    //    const auto imageResolution = hub::Resolution { { 512, 512 }, hub::Format::Y16 };

    //    const hub::SensorSpec sensorSpec( "sensorName",
    //                                      { { { 1 }, hub::Format::DOF6 }, imageResolution } );

    //    hub::io::CyclicBuff cyclicBuff;

    //    constexpr int imageSize = 512 * 512 * 2;

    //    hub::OutputSensor outputSensor( sensorSpec, hub::io::Ram( cyclicBuff ) );
    //    hub::Dof6 dof6( 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0 );
    //    CHECK( dof6.get_x() == 1.0 );
    //    CHECK( dof6.get_y() == 2.0 );
    //    CHECK( dof6.get_z() == 3.0 );
    //    CHECK( dof6.get_w0() == 4.0 );
    //    CHECK( dof6.get_w1() == 5.0 );
    //    CHECK( dof6.get_w2() == 6.0 );
    //    CHECK( dof6.get_w3() == 7.0 );

    //    unsigned char imageData[imageSize] = { 0 };
    //    for ( int i = 0; i < imageSize; ++i ) {
    //        imageData[i] = i % 256;
    //    }

    //    outputSensor << ( hub::Acquisition { 0, 0 }
    //                      << std::move( dof6 )
    //                      << hub::Measure( reinterpret_cast<unsigned char*>( imageData ),
    //                                       imageSize,
    //                                       imageResolution ) );

    //    hub::InputSensor inputSensor( ( hub::io::Ram( cyclicBuff ) ) );

    //    auto acq = inputSensor.getAcquisition();
    //    CHECK( acq.getSize() == 28 + imageSize );
    //    const auto& measures = acq.getMeasures();

    //    {
    //        const hub::Dof6 dof62( measures.at( 0 ) );
    //        CHECK( dof62.get_x() == 1.0 );
    //        CHECK( dof62.get_y() == 2.0 );
    //        CHECK( dof62.get_z() == 3.0 );
    //        CHECK( dof62.get_w0() == 4.0 );
    //        CHECK( dof62.get_w1() == 5.0 );
    //        CHECK( dof62.get_w2() == 6.0 );
    //        CHECK( dof62.get_w3() == 7.0 );
    //    }

    //    const hub::Dof6 dof63( measures.at( 0 ) );
    //    CHECK( dof63.get_x() == 1.0 );
    //    CHECK( dof63.get_y() == 2.0 );
    //    CHECK( dof63.get_z() == 3.0 );
    //    CHECK( dof63.get_w0() == 4.0 );
    //    CHECK( dof63.get_w1() == 5.0 );
    //    CHECK( dof63.get_w2() == 6.0 );
    //    CHECK( dof63.get_w3() == 7.0 );

    //    const auto& image = measures.at( 1 );
    //    CHECK( image.m_size == imageSize );
    //    CHECK( image.m_data != imageData );

    //    for ( int i = 0; i < imageSize; ++i ) {
    //        CHECK( image.m_data[i] == i % 256 );
    //    }
}
