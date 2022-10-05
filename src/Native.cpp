#include "Native.hpp"

/* Native.cpp : Defines the exported functions for the DLL.
 */

#include <iostream>

#include "IO/Stream.hpp"
#include "Net/ClientSocket.hpp"
#include <cstring>

namespace hub {
namespace native {

InputSensor* createInputSensor( const char* streamName, const char* ipv4, int port ) {
    std::cout << "[Native] createInputSensor( " << streamName << ")" << std::endl;
    InputSensor* inputSensor = nullptr;
    try {
        inputSensor =
            new InputSensor( io::InputStream( streamName, "", net::ClientSocket( ipv4, port ) ) );
    }
    catch ( std::exception& e ) {
        std::cout << "[Native] createInputSensor : catch exception : " << e.what() << std::endl;
        return nullptr;
    }
    return inputSensor;
}

void freeInputSensor( InputSensor* inputSensor ) {
    assert( inputSensor != nullptr );
    std::cout << "[Native] freeinputSensor( " << inputSensor << ")" << std::endl;
    delete inputSensor;
}

int getAcquisitionSize( const InputSensor* inputSensor ) {
    assert( inputSensor != nullptr );
    return static_cast<int>( inputSensor->m_spec.m_acquisitionSize );
}

Acquisition* getAcquisition( const InputSensor* inputSensor ) {
    assert( inputSensor != nullptr );

    std::cout << "[Native] getAcquisition( " << inputSensor << ")" << std::endl;
    Acquisition* ret = nullptr;

    try {
        auto acq = inputSensor->getAcquisition();
        std::cout << "[Native] get acq : " << acq << std::endl;
        ret = new Acquisition( std::move( acq ) );
    }
    catch ( std::exception& e ) {
        std::cout << "[Native] getAcquisition : catch exception : " << e.what() << std::endl;
        return nullptr;
//        return ret;
    }
    return ret;
}

void freeAcquisition( Acquisition* acquisition ) {
    delete acquisition;
}

void acquisition_getMeasure( const Acquisition* acquisition, unsigned char* data, int iMeasure ) {
    assert( acquisition != nullptr );
    //    std::cout << "[Native] getData( " << acquisition << ")" << std::endl;
    //    try {
    //        auto acq = acquisition->getAcquisition();
    //        std::cout << "[Native] get acq : " << acq << std::endl;

    std::cout << "[Native] copying data " << std::endl;
    assert( iMeasure < acquisition->getMeasures().size() );
    const auto& measure = acquisition->getMeasures().at( iMeasure );
    memcpy( data, measure.m_data, measure.m_size );
    //    }
    //    catch ( std::exception& e ) {
    //        std::cout << "[Native] getAcquisition : catch exception : " << e.what() << std::endl;
    //        return false;
    //    }

    //    return true;
}

// bool getData( const InputSensor* inputSensor, unsigned char* data, int iMeasure ) {
//     assert( inputSensor != nullptr );

//    std::cout << "[Native] getData( " << inputSensor << ")" << std::endl;

//    try {
//        auto acq = inputSensor->getAcquisition();
//        std::cout << "[Native] get acq : " << acq << std::endl;

//        std::cout << "[Native] copying data " << std::endl;
//        assert( iMeasure < acq.getMeasures().size() );
//        const auto& measure = acq.getMeasures().at( iMeasure );
//        memcpy( data, measure.m_data, measure.m_size );
//    }
//    catch ( std::exception& e ) {
//        std::cout << "[Native] getAcquisition : catch exception : " << e.what() << std::endl;
//        return false;
//    }

//    return true;
//}

Viewer*
createViewer( void ( *onNewStreamer )( const char* streamName, const SensorSpec* sensorSpec ),
              void ( *onDelStreamer )( const char* streamName, const SensorSpec* sensorSpec ),
              void ( *onServerConnected )(),
              void ( *onServerDisconnected )(),
              const char* ipv4,
              int port ) {

    //    Viewer* viewer = new Viewer( "127.0.0.1", 4042, {}, {} );
    //    return viewer;

    auto onNewStreamerCpp = [=]( const std::string& sensorName, const SensorSpec& sensorSpec ) {
        onNewStreamer( sensorName.c_str(), &sensorSpec );
    };
    auto onDelStreamerCpp = [=]( const std::string& sensorName, const SensorSpec& sensorSpec ) {
        onDelStreamer( sensorName.c_str(), &sensorSpec );
    };
    //    auto onServerConnectedCpp = [=](  ) {
    //        onServerConnected();
    //    };
    //    auto onServerDisconnectedCpp = [=](  ) {
    //        onServerDisconnected();
    //    };
    //    Viewer * viewer = new Viewer( onNewStreamerCpp, onDelStreamerCpp, onServerConnectedCpp,
    //    onServerDisconnectedCpp, std::string(ipv4), port);
    Viewer* viewer = new Viewer(
        onNewStreamerCpp, onDelStreamerCpp, onServerConnected, onServerDisconnected, ipv4, port );
    return viewer;
}

void freeViewer( Viewer* viewer ) {
    assert( viewer != nullptr );
    delete viewer;
}

void sensorSpec_getSensorName( const SensorSpec* sensorSpec, char* sensorName, int* strLen ) {
    //    sensorName = sensorSpec->m_sensorName.c_str();
    //    sensorName = new char[sensorSpec->m_sensorName.size() + 1];
    *strLen = sensorSpec->m_sensorName.size();
    memcpy( sensorName, sensorSpec->m_sensorName.c_str(), *strLen + 1 );
    sensorName[*strLen] = 0;
    //    sensorName = sensorSpec->m_sensorName.c_str();
}
////void passString([out] LPStr *str);
// const char * sensorSpec_getSensorName(const SensorSpec* sensorSpec) {
////    sensorName = sensorSpec->m_sensorName.c_str();
////    sensorName = new char[sensorSpec->m_sensorName.size() + 1];
//    return "hello";
//    return sensorSpec->m_sensorName.c_str();
////    const int len = sensorSpec->m_sensorName.size();
////    memcpy(sensorName, sensorSpec->m_sensorName.c_str(), len + 1);
////    sensorName[len] = 0;
////    sensorName = sensorSpec->m_sensorName.c_str();
//}

int sensorSpec_getResolutionSize( const SensorSpec* sensorSpec, int iResolution ) {
    return SensorSpec::computeAcquisitionSize( sensorSpec->m_resolutions.at( iResolution ) );
}

int sensorSpec_getResolutionsSize( const SensorSpec* sensorSpec ) {
    return sensorSpec->m_resolutions.size();
}

int sensorSpec_getAcquisitionSize( const SensorSpec* sensorSpec ) {
    return sensorSpec->m_acquisitionSize;
}

int sensorSpec_getFormat( const SensorSpec* sensorSpec, int iResolution ) {
    assert( iResolution < sensorSpec->m_resolutions.size() );
    return static_cast<int>( sensorSpec->m_resolutions.at( iResolution ).second );
}

int sensorSpec_getDimensionsSize( const SensorSpec* sensorSpec, int iResolution ) {
    assert( iResolution < sensorSpec->m_resolutions.size() );
    return sensorSpec->m_resolutions.at( iResolution ).first.size();
}

int sensorSpec_getDimension( const SensorSpec* sensorSpec, int iResolution, int iDimension ) {
    assert( iResolution < sensorSpec->m_resolutions.size() );
    assert( iDimension < sensorSpec->m_resolutions.at( iResolution ).first.size() );
    return sensorSpec->m_resolutions.at( iResolution ).first.at( iDimension );
}

void sensorSpec_getResolutionsStr( const SensorSpec* sensorSpec, char* resolutionsStr ) {
    const auto& resolutionsString = SensorSpec::resolutions2string( sensorSpec->m_resolutions );
    //    resolutionsStr = new char[resolutionsString.size() + 1];
    const int len = resolutionsString.size();
    memcpy( resolutionsStr, resolutionsString.c_str(), len + 1 );
    resolutionsStr[len] = 0;
}

void sensorSpec_getMetaDataStr( const SensorSpec* sensorSpec, char* metaDataStr ) {
    const auto& metaDataString = SensorSpec::metaData2string( sensorSpec->m_metaData, true );
    //    metaDataStr = new char[metaDataString.size() + 1];
    const int len = metaDataString.size();
    memcpy( metaDataStr, metaDataString.c_str(), len + 1 );
    metaDataStr[len] = 0;
}

SensorSpec* sensorSpec_copy( const SensorSpec* source ) {
    return new SensorSpec( source->m_sensorName, source->m_resolutions, source->m_metaData );
}

void freeSensorSpec( SensorSpec* sensorSpec ) {
    delete sensorSpec;
}

const SensorSpec::MetaData* sensorSpec_getMetaData( const SensorSpec* sensorSpec ) {
    return &sensorSpec->m_metaData;
}

bool metaData_getString( const SensorSpec::MetaData* metaData,
                         const char* metaName,
                         char* output,
                         int* strLen ) {
    if ( metaData->find( metaName ) == metaData->end() ) return false;

    const char* meta = std::any_cast<const char*>( metaData->at( metaName ) );
    //    output = (char*)meta;
    *strLen = strlen( meta );
    memcpy( output, meta, *strLen + 1 );
    output[*strLen] = 0;
    return true;
}

bool metaData_getMat4( const SensorSpec::MetaData* metaData, const char* metaName, float* output ) {
    if ( metaData->find( metaName ) != metaData->end() ) {
        const float* array = std::any_cast<const float*>( metaData->at( metaName ) );
        memcpy( output, array, 64 );
        return true;
        //        m_transform        = Eigen::Map<Eigen::Matrix4f>( (float*)array ); // Column-major
    }
    return false;
}

long long acquisition_getStart( const Acquisition* acquisition ) {
    return acquisition->m_start;
}

bool metaData_exists( const SensorSpec::MetaData* metaData, const char* metaName ) {
    return metaData->find( metaName ) != metaData->end();
}

int metaData_getInt( const SensorSpec::MetaData* metaData, const char* metaName ) {
    assert( metaData->find( metaName ) != metaData->end() );
    return std::any_cast<int>( metaData->at( metaName ) );
}

unsigned int metaData_getUInt( const SensorSpec::MetaData* metaData, const char* metaName ) {
    assert( metaData->find( metaName ) != metaData->end() );
    return std::any_cast<unsigned int>( metaData->at( metaName ) );
}

// bool getAcquisition( InputSensor* inputSensor,
//                      long long* start,
//                      long long* end,
//                      unsigned char* data ) {
//     assert( inputSensor != nullptr );

//    std::cout << "[Native] getAcquisition( " << inputSensor << ")" << std::endl;
//    try {
//        auto acq = inputSensor->getAcquisition();
//        //    Acq acq;
//        *start = acq.start;
//        *end   = acq.mBackendTimeOfArrival;
//        //    data = acq.mData;
//        memcpy( data, acq.mData, inputSensor->spec.acquisitonSize );
//    }
//    catch ( std::exception& e ) {
//        std::cout << "[Native] getAcquisition : catch exception : " << e.what() << std::endl;
//        return false;
//    }

//    return true;
//}

} // namespace native
} // namespace hub
