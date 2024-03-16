#include "test_common.hpp"

#include <core/io/Archive.hpp>
#include <io/Header.hpp>

#include <sensor/SensorSpec.hpp>

TEST_CASE( "Test io sensorSpec" ) {
    TEST_BEGIN()

    const std::string meshPath = HUB_PROJECT_DIR "data/assets/";
    // const hub::data::Mesh mesh( meshPath + "Bunny" );
    const hub::data::Mesh mesh( meshPath + "sensor" );

    hub::MetaData metaData;

    using Resolution = hub::MatrixTs<int, bool, hub::MatrixXD<char, 10>>;

    const hub::sensor::SensorSpec refSensorSpec( FILE_NAME, Resolution(), metaData );

    hub::io::Archive archive;

    // const hub::io::Header header = hub::io::make_header(refSensorSpec);

    std::cout << "-------------------------------- writing header -------------------------- "
              << std::endl;

    archive.write( refSensorSpec );

    std::cout << std::endl;
    std::cout << "-------------------------------- reading header -------------------------- "
              << std::endl;

    hub::sensor::SensorSpec sensorSpec_read;

    archive.read( sensorSpec_read );

    // CHECK(header == header_read);
    CHECK( sensorSpec_read == refSensorSpec );

    TEST_END()
}
