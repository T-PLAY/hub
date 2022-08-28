#include <catch2/catch_test_macros.hpp>

#include <IO/File.hpp>
#include <IO/Stream.hpp>
#include <InputSensor.hpp>
#include <Net/ClientSocket.hpp>
//#include <Net/Server.hpp>
#include <OutputSensor.hpp>
#include <Server.hpp>

#include <filesystem>

TEST_CASE( "Server test : direct stream" ) {

    const std::string ipv4 = "127.0.0.1";
    constexpr int port     = 5001;
    constexpr int nLoop    = 2;

    std::vector<hub::Acquisition> acqs;
    constexpr int nAcqs    = 20;
    constexpr int dataSize = 9;
    for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
        unsigned char data[dataSize];
        for ( int i = 0; i < dataSize; ++i ) {
            data[i] = iAcq + i;
        }
//        acqs.emplace_back( iAcq + 1, iAcq + 2, data, dataSize );
        acqs.emplace_back(iAcq + 1, iAcq + 2) << hub::Measure(data, dataSize);

    }

    std::cout << "[Test] ############################### server start" << std::endl;
    Server server( port );
    server.setMaxClients( 2 * nLoop );
    server.asyncRun();
    //    std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    std::cout << "[Test] server end ------------------------------" << std::endl;

    for ( int iLoop = 0; iLoop < nLoop; ++iLoop ) {
        {
            std::cout << "[Test] ############################### outputStream start" << std::endl;
            //    INFO( "OutputStream" );
            //    {

            hub::OutputSensor outputSensor(
                { "sensorName", {{{3}, hub::SensorSpec::Format::BGR8}} },
                hub::io::OutputStream( "stream", hub::net::ClientSocket( ipv4, port ) ) );

            auto& outputSensorSpec = outputSensor.m_spec;
            CHECK( outputSensorSpec.m_acquisitionSize == dataSize );
            CHECK( outputSensorSpec.m_sensorName == "sensorName" );
            CHECK( outputSensorSpec.m_resolutions.size() == 1);
            CHECK( outputSensorSpec.m_resolutions[0].first.size() == 1 );
            CHECK( outputSensorSpec.m_resolutions[0].first.at( 0 ) == 3 );
            CHECK( outputSensorSpec.m_resolutions[0].second == hub::SensorSpec::Format::BGR8 );
            std::cout << "[Test] outputStream end ---------------------------------" << std::endl;

            //        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            {
                std::cout << "[Test] ############################### inputStream start"
                          << std::endl;
                //    INFO( "InputStream" );
                //    {
                hub::InputSensor inputSensor(
                    hub::io::InputStream( "stream", "", hub::net::ClientSocket( ipv4, port ) ) );

                const auto& inputSensorSpec = inputSensor.m_spec;
                CHECK( inputSensorSpec.m_acquisitionSize == dataSize );
                CHECK( inputSensorSpec.m_sensorName == "sensorName" );
            CHECK( inputSensorSpec.m_resolutions.size() == 1);
            CHECK( inputSensorSpec.m_resolutions[0].first.size() == 1 );
            CHECK( inputSensorSpec.m_resolutions[0].first.at( 0 ) == 3 );
            CHECK( inputSensorSpec.m_resolutions[0].second == hub::SensorSpec::Format::BGR8 );

                std::cout << "[Test] ############################### send acquisitions"
                          << std::endl;
                for ( const auto& acq : acqs ) {
                    outputSensor << acq;
                }

                std::cout << "[Test] ############################### get acquisitions" << std::endl;
                //        const auto& inputAcqs = inputSensor.getAllAcquisitions();
                std::cout << "[Test] ############################### compare " << std::endl;
                //        assert( inputAcqs.size() == nAcqs );
                for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
                    //                        std::cout << "[Test] compare acq " << iAcq <<
                    //                        std::endl;
                    auto acq = inputSensor.getAcquisition();
                    CHECK( acq == acqs[iAcq] );
                }
                std::cout << "[Test] inputStream end ---------------------------------"
                          << std::endl;
            }
                    outputSensor << acqs[0];
        }
        std::cout << "[Test] inputStream deleted *******************************" << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        std::cout
            << "[Test] xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx end "
               "loop "
            << iLoop << std::endl;
    }
    std::cout << "[Test] outputStream deleted *******************************" << std::endl;
    //    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

    server.stop();
}
