#include <catch2/catch_test_macros.hpp>

#include <IO/File.hpp>
#include <InputSensor.hpp>
#include <OutputSensor.hpp>
#include <Net/ClientSocket.hpp>

#include <filesystem>

TEST_CASE( "Server test" ) {

    const std::string ipv4 = "127.0.0.1";
    constexpr int port = 5000;

    std::vector<hub::Acquisition> acqs;
    constexpr int nAcqs = 100;
    for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
        unsigned char data[3] = {
            (unsigned char)iAcq, (unsigned char)( iAcq + 1 ), (unsigned char)( iAcq + 2 ) };
        acqs.emplace_back( iAcq, iAcq, data, 3 );
    }

    std::cout << "outputStream start" << std::endl;
    INFO( "OutputStream" );
    {

        hub::OutputSensor outputSensor( { "sensorName", hub::SensorSpec::Format::BGR8, { 1 } },
                                        hub::io::File( hub::net::ClientSocket(ipv4, port) ) );

        auto& sensorSpec = outputSensor.spec;
        CHECK( sensorSpec.acquisitonSize == 3 );
        CHECK( sensorSpec.sensorName == "sensorName" );
        CHECK( sensorSpec.dims.size() == 1 );
        CHECK( sensorSpec.dims.at( 0 ) == 1 );
        CHECK( sensorSpec.format == hub::SensorSpec::Format::BGR8 );

        for ( const auto& acq : acqs ) {
            outputSensor << acq;
        }
    }
    std::cout << "outputStream end ################################" << std::endl;

//    std::cout << "inputStream start" << std::endl;
//    INFO( "InputStream" );
//    {
//        hub::InputSensor inputSensor( hub::io::File( std::fstream( filename, std::ios::in ) ) );

//        const auto& sensorSpec = inputSensor.spec;
//        CHECK( sensorSpec.acquisitonSize == 3 );
//        CHECK( sensorSpec.sensorName == "sensorName" );
//        CHECK( sensorSpec.dims.size() == 1 );
//        CHECK( sensorSpec.dims.at( 0 ) == 1 );
//        CHECK( sensorSpec.format == hub::SensorSpec::Format::BGR8 );
//        const auto& inputAcqs = inputSensor.getAllAcquisitions();
//        for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
//            CHECK( inputAcqs[iAcq] == acqs[iAcq] );
//        }
//    }
//    std::cout << "inputStream end #################################" << std::endl;

}
