/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/09/28

#pragma once

#include <filesystem>

#include "InputSensor.hpp"
#include "OutputSensor.hpp"
#include "SensorSpec.hpp"

namespace hub {
namespace sensor {
namespace utils {

///
/// \brief The SpecAcqs class
///
class SpecAcqs
{
  public:
    SpecAcqs( const SpecAcqs& ) = delete;

    ///
    /// \brief SpecAcqs
    ///
    SpecAcqs( SpecAcqs&& )      = default;

    ///
    /// \brief SpecAcqs
    /// \param sensorSpec
    ///
    SpecAcqs( const hub::sensor::SensorSpec& sensorSpec ) : m_sensorSpec { sensorSpec } {};

    ///
    /// \brief m_sensorSpec
    ///
    hub::sensor::SensorSpec m_sensorSpec;

    ///
    /// \brief m_acqs
    ///
    std::vector<hub::sensor::Acquisition> m_acqs;
};





template <class Input>
std::vector<SpecAcqs> synchonizeInputs( std::vector<Input>& inputs ) {
    std::vector<SpecAcqs> specAcqs;

    // Todo minimize inputSensor instance due of sensorSpec weight (Mesh inside ~ 1Go)
    for ( auto& input : inputs ) {
        hub::sensor::InputSensor inputSensor( input );
        const auto& metaData = inputSensor.getSpec().getMetaData();
        if ( metaData.find( "parent" ) != metaData.end() ) {
            const auto* parentName = metaData.at( "parent" ).get<const char*>();

            for ( auto& input2 : inputs ) {
                hub::sensor::InputSensor inputSensor2( input2 );
                const auto& parentSensorName = inputSensor2.getSpec().getSensorName();
                if ( parentSensorName == parentName ) {
                    std::cout << inputSensor.getSpec().getSensorName() << " -> " << parentName
                              << std::endl;
                    hub::sensor::InputSensor inputSyncSensor( input2, input );
                    specAcqs.push_back( inputSyncSensor.getSpec() );

                    auto& specAcq = specAcqs.back();
                    inputSyncSensor.fillAllAcquisitions( specAcq.m_acqs );
                    specAcq.m_sensorSpec.getMetaData()["nAcq"] = (int)specAcq.m_acqs.size();
                }
            }
        }
    }

    return specAcqs;
}

static std::vector<hub::input::InputFile> getInputFiles( const std::string& dir ) {
    assert( std::filesystem::exists( dir ) );
    assert( std::filesystem::is_directory( dir ) );
    std::filesystem::path path( dir );

    std::vector<hub::input::InputFile> inputFiles;
    assert( std::filesystem::exists( path ) );
    assert( std::filesystem::is_directory( path ) );

    for ( const auto& filePath : std::filesystem::directory_iterator( path ) ) {
        assert( std::filesystem::exists( filePath ) );
        if ( !std::filesystem::is_regular_file( filePath ) ) continue;
        assert( std::filesystem::is_regular_file( filePath ) );

        inputFiles.emplace_back( filePath.path().string() );
    }

    return inputFiles;
}

static void synchronizePath( const std::string& dir ) {
    auto inputFiles = getInputFiles( dir );

    const auto syncDir = dir + "sync/";
    if ( std::filesystem::exists( syncDir ) ) {
        std::filesystem::path( syncDir ).remove_filename();
    }
    std::filesystem::create_directory( syncDir );

    const auto& specAcqs = synchonizeInputs( inputFiles );
    for ( const auto& specAcq : specAcqs ) {
        const auto& sensorSpec = specAcq.m_sensorSpec;
        const auto& acqs       = specAcq.m_acqs;
        const std::string syncFile =
            syncDir + specAcq.m_sensorSpec.getSensorName() + "." + HUB_EXTENSION;
        hub::sensor::OutputSensor outputSensor(
            sensorSpec, hub::output::OutputFile( hub::io::make_header( sensorSpec ), syncFile ) );
        outputSensor.fill( acqs );
    }


    // const auto& sensorName = pair.first;


    // const char* parent = metaData.at( "parent" ).getConstCharPtr();
    // const char* parent = metaData.at( "parent" ).get<const char*>();


    // const auto& parentSensorSpec = parentInputSensor.getSpec();
    // const auto& parentSensorName = parentSensorSpec.getSensorName();

    // HUB_EXTENSION,




}

} // namespace utils
} // namespace sensor
} // namespace hub
