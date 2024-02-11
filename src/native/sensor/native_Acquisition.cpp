

#include <cassert>
#include <sstream>

#include <sensor/Acquisition.hpp>

#define HUB_CPP_SOURCE
#include "native_Acquisition.hpp"

namespace hub {
namespace native {

void freeAcquisition( sensor::Acquisition* acquisition ) {
    delete acquisition;
}

void acquisition_getMeasure( const sensor::Acquisition* acquisition, unsigned char* data, int iMeasure ) {
    assert( acquisition != nullptr );

    const auto & nodes = acquisition->getNodes();
    assert( iMeasure < nodes.size() - 2 );
    const auto& node = nodes.at( 2 + iMeasure );
    //const auto& measure = acquisition->getMeasures().at( iMeasure );
    const auto* acqData = acquisition->getData( 2 + iMeasure );
    memcpy( data, acqData, node.getSize());
}

long long acquisition_getStart( const sensor::Acquisition* acquisition ) {
    return acquisition->getStart();
}

void acquisition_to_string( const sensor::Acquisition* acquisition, char* str, int* strLen ) {
    std::stringstream sstr;
    sstr << *acquisition;
    const std::string& stdString = sstr.str();

    *strLen = stdString.size();
#if CPLUSPLUS_VERSION == 20
    memcpy( sensorName, sensorSpec->getSensorName().data(), *strLen + 1 );
#else
    memcpy( str, stdString.c_str(), *strLen + 1 );
#endif
    str[*strLen] = 0;
}


} // namespace native
} // namespace hub
