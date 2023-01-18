
#include <catch2/catch_test_macros.hpp>

#include <IO/File.hpp>
#include <IO/Ram.hpp>
#include <InputSensor.hpp>
#include <OutputSensor.hpp>

TEST_CASE( "Measures test" ) {

    const auto imageResolution = hub::Resolution { { 512, 512 }, hub::Format::Y16 };

    const hub::SensorSpec sensorSpec( "sensorName",
                                      { { { 1 }, hub::Format::DOF6 }, imageResolution } );

    hub::io::CyclicBuff cyclicBuff;

    constexpr int imageSize = 512 * 512 * 2;

    hub::OutputSensor outputSensor( sensorSpec, hub::io::Ram( cyclicBuff ) );
    hub::Dof6 dof6( 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0 );
    CHECK( dof6.m_x == 1.0 );
    CHECK( dof6.m_y == 2.0 );
    CHECK( dof6.m_z == 3.0 );
    CHECK( dof6.m_w0 == 4.0 );
    CHECK( dof6.m_w1 == 5.0 );
    CHECK( dof6.m_w2 == 6.0 );
    CHECK( dof6.m_w3 == 7.0 );

    unsigned char imageData[imageSize] = { 0 };
    for ( int i = 0; i < imageSize; ++i ) {
        imageData[i] = i % 256;
    }

    outputSensor << ( hub::Acquisition { 0, 0 }
                      << std::move( dof6 )
                      << hub::Measure( (unsigned char*)imageData, imageSize, imageResolution ) );

    hub::InputSensor inputSensor( ( hub::io::Ram( cyclicBuff ) ) );

    auto acq = inputSensor.getAcquisition();
    CHECK( acq.getSize() == 28 + imageSize );
    const auto& measures = acq.getMeasures();

    {
        const hub::Dof6& dof62 = measures.at( 0 );
        CHECK( dof62.m_x == 1.0 );
        CHECK( dof62.m_y == 2.0 );
        CHECK( dof62.m_z == 3.0 );
        CHECK( dof62.m_w0 == 4.0 );
        CHECK( dof62.m_w1 == 5.0 );
        CHECK( dof62.m_w2 == 6.0 );
        CHECK( dof62.m_w3 == 7.0 );
    }

    hub::Dof6 dof63 = measures.at( 0 );
    CHECK( dof63.m_x == 1.0 );
    CHECK( dof63.m_y == 2.0 );
    CHECK( dof63.m_z == 3.0 );
    CHECK( dof63.m_w0 == 4.0 );
    CHECK( dof63.m_w1 == 5.0 );
    CHECK( dof63.m_w2 == 6.0 );
    CHECK( dof63.m_w3 == 7.0 );

    const auto& image = measures.at( 1 );
    CHECK( image.m_size == imageSize );
    CHECK( image.m_data != imageData );

    for ( int i = 0; i < imageSize; ++i ) {
        CHECK( image.m_data[i] == i % 256 );
    }
}
