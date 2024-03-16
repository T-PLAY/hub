#include "test_common.hpp"

#include <core/io/Archive.hpp>
#include <io/Header.hpp>

#include <sensor/SensorSpec.hpp>


TEST_CASE( "Test io memory" ) {
    TEST_BEGIN()

    const std::string meshPath = HUB_PROJECT_DIR "data/assets/";
    // const hub::data::Mesh mesh( meshPath + "Bunny" );
    const hub::data::Mesh mesh( meshPath + "sensor" );

    hub::MetaData metaData;
    // metaData["asset"] = mesh;

    // using Resolution   = hub::MatrixTs<int, bool, hub::MatrixXD<char, 10>>;
    using Resolution   = int;
    // using OutputSensor = hub::sensor::OutputSensorT<Resolution, hub::output::OutputFile>;
    // using OutputSensor = hub::sensor::OutputSensorT<Resolution>;

    // using Acquisition  = OutputSensor::Acquisition;
    // std::vector<Acquisition> refAcqs;

    const hub::sensor::SensorSpec refSensorSpec( "AAA", Resolution(), metaData );

    hub::io::Memory memory;
    const auto & datas = memory.getData();
    CHECK(datas.size() == 0);
    memory.write(refSensorSpec);
    std::cout << "memory : " << memory.getData() << std::endl;

    // const auto header = hub::io::make_header(refSensorSpec);
    // std::cout << "header : " << header.getUserDefined() << std::endl;



    TEST_END()
}
