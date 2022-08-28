
#include <catch2/catch_test_macros.hpp>

#include <IO/File.hpp>
#include <IO/Ram.hpp>
#include <InputSensor.hpp>
#include <OutputSensor.hpp>

TEST_CASE( "Resolutions test" ) {

    std::vector<std::pair<hub::SensorSpec::Resolutions, size_t>> m_resolutionss;
    m_resolutionss.push_back( { { { { 1 }, hub::SensorSpec::Format::BGR8 } }, 3 } );
    m_resolutionss.push_back(
        { { { { 192, 512 }, hub::SensorSpec::Format::BGR8 } }, 3 * 192 * 512 } );
    m_resolutionss.push_back( { { { { 1, 2, 3, 4, 5 }, hub::SensorSpec::Format::Y8 },
                                  { { 100 }, hub::SensorSpec::Format::BGR8 } },
                                1 * 2 * 3 * 4 * 5 + 300 } );

    for ( const auto& pair : m_resolutionss ) {
        const auto& resolutions = pair.first;
        const auto& size        = pair.second;

        const hub::SensorSpec sensorSpec( "sensorName", resolutions );

        hub::io::CyclicBuff cyclicBuff;

        hub::OutputSensor outputSensor( sensorSpec, hub::io::Ram( cyclicBuff ) );
        const hub::SensorSpec& outputSensorSpec = outputSensor.m_spec;
        CHECK( outputSensorSpec.m_acquisitionSize == size );
        CHECK( outputSensorSpec.m_sensorName == "sensorName" );
        CHECK( outputSensorSpec.m_resolutions.size() == resolutions.size() );
        for ( int i = 0; i < resolutions.size(); ++i ) {
            CHECK( outputSensorSpec.m_resolutions[i].first.size() == resolutions[i].first.size() );
            for ( int j = 0; j < resolutions[i].first.size(); ++j ) {
                CHECK( outputSensorSpec.m_resolutions[i].first.at( j ) ==
                       resolutions[i].first.at( j ) );
            }
            CHECK( outputSensorSpec.m_resolutions[i].second == resolutions[i].second );
        }

        hub::InputSensor inputSensor( ( hub::io::Ram( cyclicBuff ) ) );
        hub::SensorSpec inputSensorSpec = inputSensor.m_spec;
        CHECK( inputSensorSpec.m_acquisitionSize == size );
        CHECK( inputSensorSpec.m_sensorName == "sensorName" );
        CHECK( inputSensorSpec.m_resolutions.size() == resolutions.size() );
        for ( int i = 0; i < resolutions.size(); ++i ) {
            CHECK( inputSensorSpec.m_resolutions[i].first.size() == resolutions[i].first.size() );
            for ( int j = 0; j < resolutions[i].first.size(); ++j ) {
                CHECK( inputSensorSpec.m_resolutions[i].first.at( j ) ==
                       resolutions[i].first.at( j ) );
            }
            CHECK( inputSensorSpec.m_resolutions[i].second == resolutions[i].second );
        }
    }
}
