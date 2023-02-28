#include <catch2/catch_test_macros.hpp>

#include <IO/File.hpp>
#include <InputSensor.hpp>
#include <OutputSensor.hpp>

#include <filesystem>
#include <thread>

TEST_CASE( "File test" ) {

    const std::string filename = "file.txt";

    std::filesystem::remove( filename );

    std::vector<hub::Acquisition> acqs;
    constexpr int nAcqs = 100;
    for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
        const unsigned char data[3] = {
            (unsigned char)iAcq, (unsigned char)( iAcq + 1 ), (unsigned char)( iAcq + 2 ) };
        acqs.push_back( hub::Acquisition( iAcq, iAcq ) );
        acqs.back() << hub::Measure( data, 3, { { 1 }, hub::Format::BGR8 } );
        CHECK( acqs.back().getSize() == 3 );
    }
    CHECK( acqs[0] != acqs[1] );

    std::cout << "outputStream start" << std::endl;
    INFO( "OutputStream" );
    {

        hub::OutputSensor outputSensor(
            hub::SensorSpec { "sensorName", { { { 1 }, hub::Format::BGR8 } } },
            hub::io::File(
                std::fstream( filename, std::ios::out | std::ios::binary | std::ios::trunc ) ) );

        const auto& sensorSpec = outputSensor.m_spec;
        CHECK( sensorSpec.getAcquisitionSize() == 3 );
        CHECK( sensorSpec.getSensorName() == "sensorName" );
        CHECK( sensorSpec.getResolutions().size() == 1 );
        CHECK( sensorSpec.getResolutions()[0].first.size() == 1 );
        CHECK( sensorSpec.getResolutions()[0].first.at( 0 ) == 1 );
        CHECK( sensorSpec.getResolutions()[0].second == hub::Format::BGR8 );

        for ( const auto& acq : acqs ) {
            outputSensor << acq;
        }
    }
    std::cout << "outputStream end ################################" << std::endl;

    //    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

    std::cout << "inputStream start" << std::endl;
    INFO( "InputStream" );
    {
        hub::InputSensor inputSensor(
            hub::io::File( std::fstream( filename, std::ios::in | std::ios::binary ) ) );

        const auto& sensorSpec = inputSensor.m_spec;
        CHECK( sensorSpec.getAcquisitionSize() == 3 );
        CHECK( sensorSpec.getSensorName() == "sensorName" );
        CHECK( sensorSpec.getResolutions().size() == 1 );
        CHECK( sensorSpec.getResolutions()[0].first.size() == 1 );
        CHECK( sensorSpec.getResolutions()[0].first.at( 0 ) == 1 );
        CHECK( sensorSpec.getResolutions()[0].second == hub::Format::BGR8 );
        std::cout << "####### compare acqs" << std::endl;
        for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
            auto acq = inputSensor.getAcquisition();
            assert( acq == acqs[iAcq] );
            CHECK( acq == acqs[iAcq] );
        }
    }
    std::cout << "inputStream end #################################" << std::endl;
}
