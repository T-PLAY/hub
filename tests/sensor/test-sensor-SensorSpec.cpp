

#include "test_common.hpp"

#include <core/io/Archive.hpp>
#include <sensor/SensorSpec.hpp>

#include <any>

TEST_CASE( "SensorSpec test" ) {
    TEST_BEGIN()

    hub::io::Archive archive;

    hub::MetaData metaData;
    metaData["hello"] = 5;
    metaData["name"]  = "gauthier";
    hub::sensor::SensorSpec sensorSpec { "sensorName", {}, metaData };
    archive.write( sensorSpec );
    hub::sensor::SensorSpec sensorSpec_read;
    archive.read( sensorSpec_read );
    assert( sensorSpec == sensorSpec_read );
    static_assert( !hub::Serializer::Serializable<hub::sensor::SensorSpec>() );
    static_assert( !hub::Serializer::Serializable<hub::Any>() );
    static_assert( !hub::Serializer::Serializable<hub::MetaData>() );

    TEST_END()
}
