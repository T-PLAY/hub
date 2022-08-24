#include <catch2/catch_test_macros.hpp>

#include <IO/Stream.hpp>
#include <IO/File.hpp>
#include <InputSensor.hpp>
#include <Net/ClientSocket.hpp>
//#include <Net/Server.hpp>
#include <OutputSensor.hpp>
#include <Server.hpp>

#include <filesystem>

TEST_CASE( "Server test : direct stream" ) {

    const std::string ipv4 = "127.0.0.1";
    constexpr int port     = 5000;

    std::vector<hub::Acquisition> acqs;
    constexpr int nAcqs = 20;
    constexpr int dataSize = 9;
    for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
        unsigned char data[dataSize];
        for (int i = 0; i <dataSize; ++i) {
            data[i] = iAcq + i;
        }
        acqs.emplace_back( iAcq + 1, iAcq + 2, data, dataSize );
    }

    std::cout << "[Test] ############################### server start" << std::endl;
    Server server( port );
    server.setMaxClients( 2 );
    server.asyncRun();
    std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    std::cout << "[Test] server end ------------------------------" << std::endl;

    {
        std::cout << "[Test] ############################### outputStream start" << std::endl;
        //    INFO( "OutputStream" );
        //    {

        hub::OutputSensor outputSensor(
            { "sensorName", hub::SensorSpec::Format::BGR8, { 3 } },
            hub::io::OutputStream( "stream", hub::net::ClientSocket( ipv4, port ) ) );

        auto& outputSensorSpec = outputSensor.spec;
        CHECK( outputSensorSpec.acquisitonSize == dataSize );
        CHECK( outputSensorSpec.sensorName == "sensorName" );
        CHECK( outputSensorSpec.dims.size() == 1 );
        CHECK( outputSensorSpec.dims.at( 0 ) == 3 );
        CHECK( outputSensorSpec.format == hub::SensorSpec::Format::BGR8 );

        //    }
        std::cout << "[Test] outputStream end ---------------------------------" << std::endl;

        std::cout << "[Test] ############################### inputStream start" << std::endl;
        //    INFO( "InputStream" );
        //    {
        hub::InputSensor inputSensor(
            hub::io::InputStream( "stream", "", hub::net::ClientSocket( ipv4, port ) ) );

        const auto& inputSensorSpec = inputSensor.spec;
        CHECK( inputSensorSpec.acquisitonSize == dataSize );
        CHECK( inputSensorSpec.sensorName == "sensorName" );
        CHECK( inputSensorSpec.dims.size() == 1 );
        CHECK( inputSensorSpec.dims.at( 0 ) == 3 );
        CHECK( inputSensorSpec.format == hub::SensorSpec::Format::BGR8 );

        std::cout << "[Test] ############################### send acquisitions" << std::endl;
        for ( const auto& acq : acqs ) {
            outputSensor << acq;
        }

        std::cout << "[Test] ############################### get acquisitions" << std::endl;
        //        const auto& inputAcqs = inputSensor.getAllAcquisitions();
        std::cout << "[Test] ############################### compare " << std::endl;
        //        assert( inputAcqs.size() == nAcqs );
        for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
//                        std::cout << "[Test] compare acq " << iAcq << std::endl;
            auto acq = inputSensor.getAcquisition();
            CHECK( acq == acqs[iAcq] );
        }
        std::cout << "[Test] inputStream end ---------------------------------" << std::endl;
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    server.stop();
}
