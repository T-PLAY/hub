
#include <sensor/SensorSpec.hpp>

#define HUB_CPP_SOURCE
#include "native_SensorSpec.hpp"

namespace hub {
namespace native {

void sensorSpec_getSensorName( const sensor::SensorSpec* sensorSpec,
                               char* sensorName,
                               int* strLen ) {
    *strLen = sensorSpec->getSensorName().size(); // todo
#if CPP_VERSION == 20
    memcpy( sensorName, sensorSpec->getSensorName().data(), *strLen + 1 );
#else
    memcpy( sensorName, sensorSpec->getSensorName().c_str(), *strLen + 1 );
#endif
    sensorName[*strLen] = 0;
}

void sensorSpec_getResolutionsStr( const sensor::SensorSpec* sensorSpec, char* resolutionsStr ) {
    const auto& resolutions = sensorSpec->getResolution();
    std::stringstream ss;
    ss << resolutions;
    const auto& resolutionsString = ss.str();
    const int len                 = resolutionsString.size(); // todo
    memcpy( resolutionsStr, resolutionsString.c_str(), len + 1 );
    resolutionsStr[len] = 0;
}

void sensorSpec_getMetaDataStr( const sensor::SensorSpec* sensorSpec, char* metaDataStr ) {
    const auto& metaDataString = hub::to_string( sensorSpec->getMetaData(), true );
    const int len              = metaDataString.size(); // todo
    memcpy( metaDataStr, metaDataString.c_str(), len + 1 );
    metaDataStr[len] = 0;
}

sensor::SensorSpec* sensorSpec_copy( const sensor::SensorSpec* source ) {
    return new sensor::SensorSpec(
        source->getSensorName(), source->getResolution(), source->getMetaData() );
}

void freeSensorSpec( sensor::SensorSpec* sensorSpec ) {
    assert( sensorSpec != nullptr );
    std::cout << "[Native] freeSensorSpec( " << sensorSpec << ")" << std::endl;
    delete sensorSpec;
}

const MetaData* sensorSpec_getMetaData( const sensor::SensorSpec* sensorSpec ) {
    return &sensorSpec->getMetaData();
}

std::string g_lastToString;

const char* to_string( const sensor::SensorSpec* sensorSpec ) {
    g_lastToString = sensorSpec->toString();
    return g_lastToString.c_str();
}

} // namespace native
} // namespace hub
