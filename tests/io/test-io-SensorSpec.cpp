#include "test_common.hpp"

#include <core/io/Archive.hpp>
#include <io/Header.hpp>

#include <sensor/SensorSpec.hpp>


TEST_CASE( "Test io header" ) {
    TEST_BEGIN()

    const std::string meshPath = HUB_PROJECT_DIR "data/assets/";
    // const hub::data::Mesh mesh( meshPath + "Bunny" );
    const hub::data::Mesh mesh( meshPath + "sensor" );

    hub::MetaData metaData;
    // metaData["asset"] = mesh;

    using Resolution   = hub::MatrixTs<int, bool, hub::MatrixXD<char, 10>>;
    // using OutputSensor = hub::sensor::OutputSensorT<Resolution, hub::output::OutputFile>;
    // using Acquisition  = OutputSensor::Acquisition;
    // std::vector<Acquisition> refAcqs;

    const hub::sensor::SensorSpec refSensorSpec( FILE_NAME, Resolution(), metaData );


    hub::io::Archive archive;

    // hub::Datas_t datas{1, 2, 3, 4, 5};

    // hub::io::Header header(10, datas);
    // const hub::io::Header header = hub::io::make_header(refSensorSpec);


    std::cout << "-------------------------------- writing header -------------------------- " << std::endl;

    // archive.write(header);
    archive.write(refSensorSpec);

    std::cout << std::endl;
    std::cout << "-------------------------------- reading header -------------------------- " << std::endl;

    // hub::io::Header header_read;
    hub::sensor::SensorSpec sensorSpec_read;

    archive.read(sensorSpec_read);


    // assert(header == header_read);
    assert(sensorSpec_read == refSensorSpec);


    TEST_END()
}
