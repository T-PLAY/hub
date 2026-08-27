

#include "core/Format.hpp"
#include "core/MetaData.hpp"

#include "data/Mesh.hpp"

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

bool metaData_getMat4( const MetaData* metaData, const char* metaName, float* output ) {
    if ( metaData->find( metaName ) != metaData->end() ) {
        const auto& mat4 = metaData->at( metaName ).get<format::Mat4>();
        memcpy( output, mat4.data(), 64 );
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

const data::Mesh *metaData_getMesh(const MetaData *metaData, const char *meshName)
{
    assert( metaData->find( meshName ) != metaData->end() );
    const auto & mesh =  metaData->at(meshName).get<data::Mesh>();
    return &mesh;
}

size_t metaData_getDataSize(const MetaData *metaData, const char *metaName, const char *type)
{
    assert( metaData->find( metaName ) != metaData->end() );

    std::string typeStr(type);
    if (typeStr == "vector<Vec3>") {
        const auto & vector = metaData->at(metaName).get<std::vector<format::Vec3>>();
        return vector.size() * sizeof(format::Vec3);
    }
    else {
        assert(false);
    }
    return 0;
}

void metaData_getData(const MetaData *metaData, const char *metaName, const char *type, unsigned char *data)
{
    assert( metaData->find( metaName ) != metaData->end() );

    std::string typeStr(type);
    if (typeStr == "vector<Vec3>") {
        const auto & vector = metaData->at(metaName).get<std::vector<format::Vec3>>();
        memcpy(data, vector.data(), vector.size() * sizeof(format::Vec3));
    }
    else {
        assert(false);
    }
}


} // namespace native
} // namespace hub
