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
    return static_cast<int>( inputSensor->m_spec.getAcquisitionSize() );
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
    }
    return ret;
}

void freeAcquisition( Acquisition* acquisition ) {
    delete acquisition;
}

void acquisition_getMeasure( const Acquisition* acquisition, unsigned char* data, int iMeasure ) {
    assert( acquisition != nullptr );

    assert( iMeasure < acquisition->getMeasures().size() );
    const auto& measure = acquisition->getMeasures().at( iMeasure );
    memcpy( data, measure.m_data, measure.m_size );
}

Viewer* createViewer( onNewStreamerFunc onNewStreamer,
                      onDelStreamerFunc onDelStreamer,
                      onServerConnectedFunc onServerConnected,
                      onServerDisconnectedFunc onServerDisconnected,
                      onNewAcquisitionFunc onNewAcquisition,
                      const char* ipv4,
                      int port ) {

    auto onNewStreamerCpp = [=]( const std::string& streamName, const SensorSpec& sensorSpec ) {
        onNewStreamer( streamName.c_str(), &sensorSpec );
        return true;
    };
    auto onDelStreamerCpp = [=]( const std::string& streamName, const SensorSpec& sensorSpec ) {
        onDelStreamer( streamName.c_str(), &sensorSpec );
    };
    auto onServerConnectedCpp = [=]( const std::string& ipv4, int port ) {
        onServerConnected( ipv4.c_str(), port );
    };
    auto onServerDisconnectedCpp = [=]( const std::string& ipv4, int port ) {
        onServerDisconnected( ipv4.c_str(), port );
    };
    auto onNewAcquisitionCpp = [=]( const std::string& streamName, const Acquisition& acq ) {
        onNewAcquisition( streamName.c_str(), &acq );
    };
    Viewer* viewer = new Viewer( onNewStreamerCpp,
                                 onDelStreamerCpp,
                                 onServerConnectedCpp,
                                 onServerDisconnectedCpp,
                                 onNewAcquisitionCpp,
                                 ipv4,
                                 port );
    return viewer;
}

void freeViewer( Viewer* viewer ) {
    assert( viewer != nullptr );
    delete viewer;
}

void viewer_setIpv4(Viewer * viewer, const char * ipv4) {
    viewer->setIpv4(ipv4);
}

void viewer_setPort(Viewer *viewer, int port)
{
    viewer->setPort(port);
}

bool viewer_isConnected(Viewer *viewer)
{
//    return viewer.
    return viewer->isConnected();
}

void sensorSpec_getSensorName( const SensorSpec* sensorSpec, char* sensorName, int* strLen ) {
    *strLen = sensorSpec->getSensorName().size();
    memcpy( sensorName, sensorSpec->getSensorName().c_str(), *strLen + 1 );
//    memcpy( sensorName, sensorSpec->getSensorName().data(), *strLen + 1 );
    sensorName[*strLen] = 0;
}

int sensorSpec_getResolutionSize( const SensorSpec* sensorSpec, int iResolution ) {
    return SensorSpec::computeAcquisitionSize( sensorSpec->getResolutions().at( iResolution ) );
}

int sensorSpec_getResolutionsSize( const SensorSpec* sensorSpec ) {
    return sensorSpec->getResolutions().size();
}

int sensorSpec_getAcquisitionSize( const SensorSpec* sensorSpec ) {
    return sensorSpec->getAcquisitionSize();
}

int sensorSpec_getFormat( const SensorSpec* sensorSpec, int iResolution ) {
    assert( iResolution < sensorSpec->getResolutions().size() );
    return static_cast<int>( sensorSpec->getResolutions().at( iResolution ).second );
}

int sensorSpec_getDimensionsSize( const SensorSpec* sensorSpec, int iResolution ) {
    assert( iResolution < sensorSpec->getResolutions().size() );
    return sensorSpec->getResolutions().at( iResolution ).first.size();
}

int sensorSpec_getDimension( const SensorSpec* sensorSpec, int iResolution, int iDimension ) {
    assert( iResolution < sensorSpec->getResolutions().size() );
    assert( iDimension < sensorSpec->getResolutions().at( iResolution ).first.size() );
    return sensorSpec->getResolutions().at( iResolution ).first.at( iDimension );
}

void sensorSpec_getResolutionsStr( const SensorSpec* sensorSpec, char* resolutionsStr ) {
    const auto& resolutionsString = SensorSpec::resolutions2string( sensorSpec->getResolutions() );
    const int len                 = resolutionsString.size();
    memcpy( resolutionsStr, resolutionsString.c_str(), len + 1 );
    resolutionsStr[len] = 0;
}

void sensorSpec_getMetaDataStr( const SensorSpec* sensorSpec, char* metaDataStr ) {
    const auto& metaDataString = SensorSpec::metaData2string( sensorSpec->getMetaData(), true );
    const int len              = metaDataString.size();
    memcpy( metaDataStr, metaDataString.c_str(), len + 1 );
    metaDataStr[len] = 0;
}

SensorSpec* sensorSpec_copy( const SensorSpec* source ) {
    return new SensorSpec( source->getSensorName(), source->getResolutions(), source->getMetaData() );
}

void freeSensorSpec( SensorSpec* sensorSpec ) {
    delete sensorSpec;
}

const SensorSpec::MetaData* sensorSpec_getMetaData( const SensorSpec* sensorSpec ) {
    return &sensorSpec->getMetaData();
}

bool metaData_getString( const SensorSpec::MetaData* metaData,
                         const char* metaName,
                         char* output,
                         int* strLen ) {
    if ( metaData->find( metaName ) == metaData->end() ) return false;

    const char* meta = std::any_cast<const char*>( metaData->at( metaName ) );
    *strLen          = strlen( meta );
    memcpy( output, meta, *strLen + 1 );
    output[*strLen] = 0;
    return true;
}

bool metaData_getMat4( const SensorSpec::MetaData* metaData, const char* metaName, float* output ) {
    if ( metaData->find( metaName ) != metaData->end() ) {
        const float* array = std::any_cast<const float*>( metaData->at( metaName ) );
        memcpy( output, array, 64 );
        return true;
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



} // namespace native
} // namespace hub