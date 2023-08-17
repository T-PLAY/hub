
#include "test-common.hpp"
#include <catch2/catch_test_macros.hpp>

#include <cmath>
#include <iostream>

#include <InputSensor.hpp>
#include <OutputSensor.hpp>
#include <SensorSpec.hpp>
#include <data/Dof6.hpp>
#include <data/Measure.hpp>
#include <server/Server.hpp>

TEST_CASE( "heavy data" ) {
    const std::string ipv4 = "127.0.0.1";
    const int port         = GET_RANDOM_PORT;
    std::cout << "[Test] ############################### server start" << std::endl;
    hub::Server server( port );
    server.setMaxClients( 2 );
    server.asyncRun();

    hub::Resolutions resolutions;

    hub::Resolution rgbResolution = { { 640, 480 }, hub::Format::RGB8 };
    resolutions.push_back( rgbResolution );

    hub::Resolution pointsResolution = { { 640, 480 }, hub::Format::XYZ32F };
    resolutions.push_back( pointsResolution );

    hub::SensorSpec::MetaData metaData;
    metaData["parent"]            = "Polhemus Patriot (sensor 1)";
    const std::string m_assetPath = HUB_DIR "/assets/";
    metaData["model"]             = hub::data::Mesh( m_assetPath + "sensor" );

    hub::OutputSensor outputSensor( hub::SensorSpec { "Sensor", resolutions, metaData },
                                    "Sensor",
//                                    hub::net::ClientSocket( ipv4, port ) );
                                    ipv4, port );

    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    hub::InputSensor inputSensor(
//        hub::io::InputStream( "Sensor", hub::net::ClientSocket( ipv4, port ) ) );
        hub::input::InputStream( "Sensor", ipv4, port ) );

    std::vector<hub::Acquisition> acqs;
    constexpr size_t nAcq = 100;

    for ( int i = 0; i < nAcq; ++i ) {
        hub::Acquisition acq { i, i };

        constexpr size_t colorSize = 640 * 480 * 3;
        unsigned char* colors      = new unsigned char[colorSize];
        for ( int j = 0; j < colorSize; ++j ) {
            colors[j] = i + j;
        }
        acq << hub::data::Measure {
                                    reinterpret_cast<unsigned char*>(colors), colorSize, { { 640, 480 }, hub::Format::RGB8 } };
        delete[] colors;

        constexpr size_t verticesSize = 640 * 480 * 3 * 4;
        unsigned char* vertices       = new unsigned char[verticesSize];
        for ( int j = 0; j < colorSize; ++j ) {
            vertices[j] = i + j;
        }

        acq << hub::data::Measure {
                                    reinterpret_cast<unsigned char*>(vertices), verticesSize, { { 640, 480 }, hub::Format::XYZ32F } };
        delete[] vertices;

        acqs.push_back( std::move( acq ) );
    }

    for ( int i = 0; i < nAcq; ++i ) {
        std::cout << "send acq " << i << std::endl;
        outputSensor << acqs[i];

        hub::Acquisition acq2;
        inputSensor >> acq2;
        std::cout << "recv acq " << i << std::endl;
        CHECK( acqs[i] == acq2 );

        const auto& measures = acq2.getMeasures();
        assert( measures.size() == 2 );

        constexpr size_t colorSize = 640 * 480 * 3;
        unsigned char* colors      = new unsigned char[colorSize];
        for ( int j = 0; j < colorSize; ++j ) {
            colors[j] = i + j;
        }
        CHECK( !memcmp( colors, measures.at( 0 ).getData(), colorSize ) );
        delete[] colors;

        constexpr size_t verticesSize = 640 * 480 * 3 * 4;
        unsigned char* vertices       = new unsigned char[verticesSize];
        for ( int j = 0; j < colorSize; ++j ) {
            vertices[j] = i + j;
        }
        CHECK( !memcmp( vertices, measures.at( 1 ).getData(), verticesSize ) );
        delete[] vertices;
    }
}
