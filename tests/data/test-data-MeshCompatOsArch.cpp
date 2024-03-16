
#include <filesystem>

#include "test_common.hpp"

#include <sensor/InputSensor.hpp>
#include <sensor/OutputSensor.hpp>

TEST_CASE( "mesh compat os and arch test" ) {
    TEST_BEGIN()

    const std::string meshPath = HUB_PROJECT_DIR "data/assets/";
    // const hub::data::Mesh mesh( meshPath + "Bunny" );
    const hub::data::Mesh refMesh( meshPath + "sensor" );
    assert( std::filesystem::exists( meshPath + "sensor.glb" ) );

    const std::string saveMesh = HUB_PROJECT_DIR "data/compat/sensor.glb";

    // Testing unique multi os/arch file
    if ( !std::filesystem::exists( saveMesh ) ) {
        std::filesystem::copy( meshPath + "sensor.glb", saveMesh );
    }

    const hub::data::Mesh mesh( meshPath + "sensor" );
    assert( refMesh == mesh );

    TEST_END()
}
