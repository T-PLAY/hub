#include <catch2/catch_test_macros.hpp>

#include <IO/File.hpp>
#include <InputSensor.hpp>
#include <OutputSensor.hpp>

#include <filesystem>

TEST_CASE( "File test" ) {

    const std::string filename = "file.txt";

    //    std::fstream fileStream( filename, std::ios::out );
    //    assert( fileStream.is_open() );

    //    hub::io::File file( std::move( fileStream ) );

    //    const hub::SensorSpec sensorSpec( "hello", hub::SensorSpec::Format::BGR8, { 1 } );
    //    hub::OutputSensor outputSensor( std::move( sensorSpec ), std::move( file ) );
    std::filesystem::remove( filename );

    std::vector<hub::Acquisition> acqs;
    constexpr int nAcqs = 100;
    for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
        unsigned char data[3] = {
            (unsigned char)iAcq, (unsigned char)( iAcq + 1 ), (unsigned char)( iAcq + 2 ) };
        acqs.emplace_back( iAcq, iAcq, data, 3 );
    }
    CHECK(acqs[0] != acqs[1]);

    std::cout << "outputStream start" << std::endl;
    INFO( "OutputStream" );
    {

        hub::OutputSensor outputSensor( { "sensorName", hub::SensorSpec::Format::BGR8, { 1 } },
                                        hub::io::File( std::fstream( filename, std::ios::out ) ) );

        auto& sensorSpec = outputSensor.m_spec;
        CHECK( sensorSpec.m_acquisitionSize == 3 );
        CHECK( sensorSpec.m_sensorName == "sensorName" );
        CHECK( sensorSpec.m_dims.size() == 1 );
        CHECK( sensorSpec.m_dims.at( 0 ) == 1 );
        CHECK( sensorSpec.m_format == hub::SensorSpec::Format::BGR8 );

        for ( const auto& acq : acqs ) {
            outputSensor << acq;
        }
    }
    std::cout << "outputStream end ################################" << std::endl;

    std::cout << "inputStream start" << std::endl;
    INFO( "InputStream" );
    {
        hub::InputSensor inputSensor( hub::io::File( std::fstream( filename, std::ios::in ) ) );

        const auto& sensorSpec = inputSensor.m_spec;
        CHECK( sensorSpec.m_acquisitionSize == 3 );
        CHECK( sensorSpec.m_sensorName == "sensorName" );
        CHECK( sensorSpec.m_dims.size() == 1 );
        CHECK( sensorSpec.m_dims.at( 0 ) == 1 );
        CHECK( sensorSpec.m_format == hub::SensorSpec::Format::BGR8 );
        const auto& inputAcqs = inputSensor.getAllAcquisitions();
        for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
            CHECK( inputAcqs[iAcq] == acqs[iAcq] );
        }
    }
    std::cout << "inputStream end #################################" << std::endl;

    //    hub::OutputSensor outputSensor2(hub::SensorSpec("hello2"), std::fstream(filename,
    //    std::ios::out));
}
