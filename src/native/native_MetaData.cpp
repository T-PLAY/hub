

// #include "sensor/SensorSpec.hpp"
#include "core/MetaData.hpp"
#include "sensor/Format.hpp"

#define HUB_CPP_SOURCE
#include "native_MetaData.hpp"

namespace hub {
namespace native {


bool metaData_getString( const MetaData* metaData,
                         const char* metaName,
                         char* output,
                         int* strLen ) {
    if ( metaData->find( metaName ) == metaData->end() ) return false;

    const char* meta = metaData->at( metaName ).get<const char*>();
    *strLen          = strlen( meta ); // todo
    memcpy( output, meta, *strLen + 1 );
    output[*strLen] = 0;
    return true;
}

bool metaData_getMat4( const MetaData* metaData,
                       const char* metaName,
                       float* output ) {
    if ( metaData->find( metaName ) != metaData->end() ) {
        // const auto& mat4 = metaData->at( metaName ).get<data::Mat4>();
        const auto& mat4 = metaData->at( metaName ).get<sensor::format::MAT4>();
        // memcpy( output, mat4.getData(), 64 );
        memcpy( output, mat4.data, 64 );
        return true;
    }
    return false;
}

bool metaData_exists( const MetaData* metaData, const char* metaName ) {
    return metaData->find( metaName ) != metaData->end();
}

int metaData_getInt( const MetaData* metaData, const char* metaName ) {
    assert( metaData->find( metaName ) != metaData->end() );
    return metaData->at( metaName ).get<int>();
}

double metaData_getDouble( const MetaData* metaData, const char* metaName ) {
    assert( metaData->find( metaName ) != metaData->end() );
    return metaData->at( metaName ).get<double>();
}

} // namespace native
} // namespace hub
