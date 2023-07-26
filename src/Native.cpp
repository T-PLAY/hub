#include "Native.hpp"

#include <iostream>

#include <cstring>
#include <sstream>

namespace hub {
namespace native {

InputSensor* createInputSensor( const char* streamName, const char* ipv4, int port ) {
    std::cout << "[Native] createInputSensor( " << streamName << ")" << std::endl;
    InputSensor* inputSensor = nullptr;
    try {
        inputSensor =
            new InputSensor( io::InputStream( streamName, net::ClientSocket( ipv4, port ) ) );
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
    return static_cast<int>( inputSensor->getSpec().getAcquisitionSize() );
}

Acquisition* getAcquisition( InputSensor* inputSensor ) {
    assert( inputSensor != nullptr );

    std::cout << "[Native] getAcquisition( " << inputSensor << ")" << std::endl;
    Acquisition* ret = nullptr;

    try {
        hub::Acquisition acq;
        *inputSensor >> acq;
        std::cout << "[Native] get acq : " << acq << std::endl;
        ret = new Acquisition( std::move( acq ) );
    }
    catch ( std::exception& e ) {
        std::cout << "[Native] getAcquisition : catch exception : " << e.what() << std::endl;
        return nullptr;
    }
    return ret;
}

///////////////////////////////////////////////////////////////////////

OutputSensor* createMat4OutputSensor( const char* sensorName, const char* ipv4, int port ) {
    std::cout << "[Native] createOutputSensor( " << sensorName << ")" << std::endl;
    OutputSensor* outputSensor = nullptr;
    try {
        SensorSpec sensorSpec( sensorName, { { { 1 }, hub::Format::MAT4 } } );
        outputSensor = new OutputSensor(
            std::move( sensorSpec ), sensorName, net::ClientSocket( ipv4, port ) );
    }
    catch ( std::exception& e ) {
        std::cout << "[Native] createOutputSensor : catch exception : " << e.what() << std::endl;
        return nullptr;
    }
    return outputSensor;
}

bool mat4OutputSensorSendAcq( OutputSensor* outputSensor, const float* input ) {
    assert( input != nullptr );
    try {
        hub::Acquisition acq( 1, 1 );
        hub::data::Mat4 mat4( input );
        acq << std::move( mat4 );
        *outputSensor << acq;
        std::cout << "[Native] send acq : " << acq << std::endl;
    }
    catch ( std::exception& e ) {
        std::cout << "[Native] getAcquisition : catch exception : " << e.what() << std::endl;
        return false;
    }
    return true;
}

void freeOutputSensor( OutputSensor* outputSensor ) {
    assert( outputSensor != nullptr );
    std::cout << "[Native] freeOutputSensor( " << outputSensor << ")" << std::endl;
    delete outputSensor;
}

///////////////////////////////////////////////////////////////////////

void freeAcquisition( Acquisition* acquisition ) {
    delete acquisition;
}

void acquisition_getMeasure( const Acquisition* acquisition, unsigned char* data, int iMeasure ) {
    assert( acquisition != nullptr );

    assert( iMeasure < acquisition->getMeasures().size() );
    const auto& measure = acquisition->getMeasures().at( iMeasure );
    memcpy( data, measure.getData(), measure.getSize() );
}

long long acquisition_getStart( const Acquisition* acquisition ) {
    return acquisition->getStart();
}

void acquisition_to_string( const Acquisition* acquisition, char* str, int* strLen ) {
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

//////////////////////////////////////////////////////////////////////////

client::Viewer* createViewer( onNewStreamerFunc onNewStreamer,
                              onDelStreamerFunc onDelStreamer,
                              onServerNotFoundFunc onServerNotFound,
                              onServerConnectedFunc onServerConnected,
                              onServerDisconnectedFunc onServerDisconnected,
                              onNewAcquisitionFunc onNewAcquisition,
                              onSetPropertyFunc onSetProperty,
                              const char* ipv4,
                              int port,
                              onLogMessageFunc onLogMessage ) {

    auto onNewStreamerCpp = [=]( const std::string& streamName, const SensorSpec& sensorSpec ) {
        return onNewStreamer( streamName.c_str(), &sensorSpec );
    };
    auto onDelStreamerCpp = [=]( const std::string& streamName, const SensorSpec& sensorSpec ) {
        onDelStreamer( streamName.c_str(), &sensorSpec );
    };
    auto onServerNotFoundCpp = [=]( const std::string& ipv4, int port ) {
        onServerNotFound( ipv4.c_str(), port );
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
    auto onSetPropertyCpp = [=]( const std::string& streamName,
                                 const std::string& objectName,
                                 int property,
                                 const Any& value ) {
        onSetProperty( streamName.c_str(), objectName.c_str(), property, &value );
    };
    auto onLogMessageCpp = [=]( const std::string& logMessage ) {
        onLogMessage( logMessage.c_str() );
    };
    client::Viewer* viewer = new client::Viewer( onNewStreamerCpp,
                                                 onDelStreamerCpp,
                                                 onServerNotFoundCpp,
                                                 onServerConnectedCpp,
                                                 onServerDisconnectedCpp,
                                                 onNewAcquisitionCpp,
                                                 onSetPropertyCpp,
                                                 ipv4,
                                                 port,
                                                 false,
                                                 onLogMessageCpp );
    return viewer;
}

void freeViewer( client::Viewer* viewer ) {
    assert( viewer != nullptr );
    delete viewer;
}

void viewer_setIpv4( client::Viewer* viewer, const char* ipv4 ) {
    viewer->setIpv4( ipv4 );
}

void viewer_setPort( client::Viewer* viewer, int port ) {
    viewer->setPort( port );
}

int viewer_getPort( client::Viewer* viewer ) {
    assert( viewer != nullptr );
    return viewer->getPort();
}

void viewer_getIpv4( client::Viewer* viewer, char* ipv4 ) {
    const auto& ipv4Str = viewer->getIpv4();
    const int len       = ipv4Str.size();
    memcpy( ipv4, ipv4Str.data(), len + 1 );
    ipv4[len] = 0;
}

bool viewer_isConnected( client::Viewer* viewer ) {
    return viewer->isConnected();
}

void sensorSpec_getSensorName( const SensorSpec* sensorSpec, char* sensorName, int* strLen ) {
    *strLen = sensorSpec->getSensorName().size();
#if CPLUSPLUS_VERSION == 20
    memcpy( sensorName, sensorSpec->getSensorName().data(), *strLen + 1 );
#else
    memcpy( sensorName, sensorSpec->getSensorName().c_str(), *strLen + 1 );
#endif
    sensorName[*strLen] = 0;
}

int sensorSpec_getResolutionSize( const SensorSpec* sensorSpec, int iResolution ) {
    return res::computeAcquisitionSize( sensorSpec->getResolutions().at( iResolution ) );
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
    const auto& resolutions = sensorSpec->getResolutions();
    std::stringstream ss;
    ss << resolutions;
    const auto& resolutionsString = ss.str();
    const int len                 = resolutionsString.size();
    memcpy( resolutionsStr, resolutionsString.c_str(), len + 1 );
    resolutionsStr[len] = 0;
}

void sensorSpec_getMetaDataStr( const SensorSpec* sensorSpec, char* metaDataStr ) {
    const auto& metaDataString = SensorSpec::to_string( sensorSpec->getMetaData(), true );
    const int len              = metaDataString.size();
    memcpy( metaDataStr, metaDataString.c_str(), len + 1 );
    metaDataStr[len] = 0;
}

SensorSpec* sensorSpec_copy( const SensorSpec* source ) {
    return new SensorSpec(
        source->getSensorName(), source->getResolutions(), source->getMetaData() );
}

void freeSensorSpec( SensorSpec* sensorSpec ) {
    assert( sensorSpec != nullptr );
    std::cout << "[Native] freeSensorSpec( " << sensorSpec << ")" << std::endl;
    delete sensorSpec;
    sensorSpec = nullptr;
}

const SensorSpec::MetaData* sensorSpec_getMetaData( const SensorSpec* sensorSpec ) {
    return &sensorSpec->getMetaData();
}

bool metaData_getString( const SensorSpec::MetaData* metaData,
                         const char* metaName,
                         char* output,
                         int* strLen ) {
    if ( metaData->find( metaName ) == metaData->end() ) return false;

    // todo any
    const char* meta = metaData->at( metaName ).getConstCharPtr();
    *strLen          = strlen( meta );
    memcpy( output, meta, *strLen + 1 );
    output[*strLen] = 0;
    return true;
}

bool metaData_getMat4( const SensorSpec::MetaData* metaData, const char* metaName, float* output ) {
    if ( metaData->find( metaName ) != metaData->end() ) {
        // todo any
        const auto& mat4 = metaData->at( metaName ).getMat4();
        memcpy( output, mat4.getData(), 64 );
        return true;
    }
    return false;
}

bool metaData_exists( const SensorSpec::MetaData* metaData, const char* metaName ) {
    return metaData->find( metaName ) != metaData->end();
}

int metaData_getInt( const SensorSpec::MetaData* metaData, const char* metaName ) {
    assert( metaData->find( metaName ) != metaData->end() );
    // todo any
    return metaData->at( metaName ).getInt();
}

double metaData_getDouble( const SensorSpec::MetaData* metaData, const char* metaName ) {
    assert( metaData->find( metaName ) != metaData->end() );
    // todo any
    return metaData->at( metaName ).getDouble();
}

double any_getDouble( const Any* any ) {
    return any->getDouble();
}

int any_getInt( const Any* any ) {
    return any->getInt();
}

} // namespace native
} // namespace hub
