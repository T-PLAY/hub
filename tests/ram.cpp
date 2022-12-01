#include <catch2/catch_test_macros.hpp>

#include <IO/Ram.hpp>
#include <InputSensor.hpp>
#include <OutputSensor.hpp>

#include <thread>

TEST_CASE( "Ram test" ) {

    hub::io::CyclicBuff cyclicBuff;

    std::vector<hub::Acquisition> acqs;
    constexpr int nAcqs = 100;
    for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
        unsigned char data[3] = {
            (unsigned char)iAcq, (unsigned char)( iAcq + 1 ), (unsigned char)( iAcq + 2 ) };
        acqs.push_back( hub::Acquisition( iAcq, iAcq ) );
        acqs.back() << hub::Measure( data, 3 );
        CHECK( acqs.back().getSize() == 3 );
    }
    CHECK( acqs[0] != acqs[1] );

    std::cout << "outputStream start" << std::endl;
    INFO( "OutputStream" );
    {

        hub::OutputSensor outputSensor(
            { "sensorName", { { { 1 }, hub::SensorSpec::Format::BGR8 } } },
            hub::io::Ram( hub::io::Ram( cyclicBuff ) ) );

        auto& sensorSpec = outputSensor.m_spec;
        CHECK( sensorSpec.getAcquisitionSize() == 3 );
        CHECK( sensorSpec.getSensorName() == "sensorName" );
        CHECK( sensorSpec.getResolutions().size() == 1 );
        CHECK( sensorSpec.getResolutions()[0].first.size() == 1 );
        CHECK( sensorSpec.getResolutions()[0].first.at( 0 ) == 1 );
        CHECK( sensorSpec.getResolutions()[0].second == hub::SensorSpec::Format::BGR8 );

        for ( const auto& acq : acqs ) {
            outputSensor << acq;
        }
    }
    std::cout << "outputStream end ################################" << std::endl;

    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

    std::cout << "inputStream start" << std::endl;
    INFO( "InputStream" );
    {
        hub::InputSensor inputSensor = hub::InputSensor( hub::io::Ram( cyclicBuff ) );

        const auto& sensorSpec = inputSensor.m_spec;
        CHECK( sensorSpec.getAcquisitionSize() == 3 );
        CHECK( sensorSpec.getSensorName() == "sensorName" );
        CHECK( sensorSpec.getResolutions().size() == 1 );
        CHECK( sensorSpec.getResolutions()[0].first.size() == 1 );
        CHECK( sensorSpec.getResolutions()[0].first.at( 0 ) == 1 );
        CHECK( sensorSpec.getResolutions()[0].second == hub::SensorSpec::Format::BGR8 );
        std::cout << "####### compare acqs" << std::endl;
        for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
            auto acq = inputSensor.getAcquisition();
            CHECK( acq == acqs[iAcq] );
        }
    }
    std::cout << "inputStream end #################################" << std::endl;
}
