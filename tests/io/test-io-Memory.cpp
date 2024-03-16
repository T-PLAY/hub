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

    using Resolution = int;

    const hub::sensor::SensorSpec refSensorSpec( "AAA", Resolution(), metaData );

    hub::io::Memory memory;
    const auto& datas = memory.getData();
    CHECK( datas.size() == 0 );
    memory.write( refSensorSpec );
    std::cout << "memory : " << memory.getData() << std::endl;

    // const auto header = hub::io::make_header(refSensorSpec);

    TEST_END()
}
