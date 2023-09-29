#include "Native.hpp"

#include <iostream>
#include <cstring>
#include <sstream>

#include "io/input/InputStream.hpp"
#include "io/output/OutputStream.hpp"

#include "data/Mat4.hpp"

namespace hub {
namespace native {

sensor::InputSensor* createInputSensor( const char* streamName, const char* ipv4, int port ) {
    std::cout << "[Native] createInputSensor( " << streamName << ")" << std::endl;
    sensor::InputSensor* inputSensor = nullptr;
    try {
        inputSensor =
            //            new InputSensor( io::InputStream( streamName, net::ClientSocket( ipv4,
            //            port ) ) );
            new sensor::InputSensor( input::InputStream( streamName, ipv4, port ) );
    }
    catch ( std::exception& e ) {
        std::cout << "[Native] createInputSensor : catch exception : " << e.what() << std::endl;
        return nullptr;
    }
    return inputSensor;
}

void freeInputSensor( sensor::InputSensor* inputSensor ) {
    assert( inputSensor != nullptr );
    std::cout << "[Native] freeinputSensor( " << inputSensor << ")" << std::endl;
    delete inputSensor;
}

int getAcquisitionSize( const sensor::InputSensor* inputSensor ) {
    assert( inputSensor != nullptr );
    return static_cast<int>( inputSensor->getSpec().getAcquisitionSize() );
}

sensor::Acquisition* getAcquisition( sensor::InputSensor* inputSensor ) {
    assert( inputSensor != nullptr );

    std::cout << "[Native] getAcquisition( " << inputSensor << ")" << std::endl;
    sensor::Acquisition* ret = nullptr;

    try {
        sensor::Acquisition acq;
        *inputSensor >> acq;
        std::cout << "[Native] get acq : " << acq << std::endl;
        ret = new sensor::Acquisition( std::move( acq ) );
    }
    catch ( std::exception& e ) {
        std::cout << "[Native] getAcquisition : catch exception : " << e.what() << std::endl;
        return nullptr;
    }
    return ret;
}

///////////////////////////////////////////////////////////////////////

sensor::OutputSensor* createMat4OutputSensor( const char* sensorName, const char* ipv4, int port ) {
    std::cout << "[Native] createOutputSensor( " << sensorName << ")" << std::endl;
    sensor::OutputSensor* outputSensor = nullptr;
    try {
        sensor::SensorSpec sensorSpec( sensorName, { { { 1 }, sensor::Format::MAT4 } } );
        //        outputSensor = new OutputSensor(
        //            //            std::move( sensorSpec ), sensorName, net::ClientSocket( ipv4,
        //            port ) ); std::move( sensorSpec ), sensorName, ipv4, port );
        outputSensor = new sensor::OutputSensor(
            //            std::move( sensorSpec ), sensorName, net::ClientSocket( ipv4, port ) );
            std::move( sensorSpec ),
//            output::OutputStream( sensorName, ipv4, port ) );
            output::OutputStream(sensorName, ipv4, port));
    }
    catch ( std::exception& e ) {
        std::cout << "[Native] createOutputSensor : catch exception : " << e.what() << std::endl;
        return nullptr;
    }
    return outputSensor;
}

bool mat4OutputSensorSendAcq( sensor::OutputSensor* outputSensor, const float* input ) {
    assert( input != nullptr );
    try {
        sensor::Acquisition acq( 1, 1 );
        data::Mat4 mat4( input );
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

void freeOutputSensor( sensor::OutputSensor* outputSensor ) {
    assert( outputSensor != nullptr );
    std::cout << "[Native] freeOutputSensor( " << outputSensor << ")" << std::endl;
    delete outputSensor;
}

///////////////////////////////////////////////////////////////////////

void freeAcquisition( sensor::Acquisition* acquisition ) {
    delete acquisition;
}

void acquisition_getMeasure( const sensor::Acquisition* acquisition, unsigned char* data, int iMeasure ) {
    assert( acquisition != nullptr );

    assert( iMeasure < acquisition->getMeasures().size() );
    const auto& measure = acquisition->getMeasures().at( iMeasure );
    memcpy( data, measure.getData(), measure.getSize() );
}

long long acquisition_getStart( const sensor::Acquisition* acquisition ) {
    return acquisition->getStart();
}

void acquisition_to_string( const sensor::Acquisition* acquisition, char* str, int* strLen ) {
    std::stringstream sstr;
    sstr << *acquisition;
    const std::string& stdString = sstr.str();

    *strLen = stdString.size(); // todo
#if CPLUSPLUS_VERSION == 20
    memcpy( sensorName, sensorSpec->getSensorName().data(), *strLen + 1 );
#else
    memcpy( str, stdString.c_str(), *strLen + 1 );
#endif
    str[*strLen] = 0;
}

//////////////////////////////////////////////////////////////////////////

// template <class InputStream>
client::Viewer* createViewer(
                        const char * name,
                              onNewStreamerFunc onNewStreamer,
                              onDelStreamerFunc onDelStreamer,
                              onServerNotFoundFunc onServerNotFound,
                              onServerConnectedFunc onServerConnected,
                              onServerDisconnectedFunc onServerDisconnected,
                              onNewAcquisitionFunc onNewAcquisition,
                              onSetPropertyFunc onSetProperty,
                              onLogMessageFunc onLogMessage,
    const char* ipv4,
    int port
    ) {

    auto onNewStreamerCpp = [=]( const std::string& streamName, const sensor::SensorSpec& sensorSpec ) {
        return onNewStreamer( streamName.c_str(), &sensorSpec );
    };
    auto onDelStreamerCpp = [=]( const std::string& streamName, const sensor::SensorSpec& sensorSpec ) {
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
    auto onNewAcquisitionCpp = [=]( const std::string& streamName, const sensor::Acquisition& acq ) {
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
    client::Viewer* viewer = new client::Viewer(
        //            ipv4,
        //                            port,
        name,
        onNewStreamerCpp,
        onDelStreamerCpp,
        onServerNotFoundCpp,
        onServerConnectedCpp,
        onServerDisconnectedCpp,
        onNewAcquisitionCpp,
        onSetPropertyCpp,
        //                                                 false,
        onLogMessageCpp, ipv4, port );
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

int viewer_getPort( const client::Viewer* viewer ) {
    assert( viewer != nullptr );
    return viewer->getPort();
}

void viewer_getIpv4( const client::Viewer* viewer, char* ipv4 ) {
    const auto& ipv4Str = viewer->getIpv4();
    const int len       = ipv4Str.size(); // todo
    memcpy( ipv4, ipv4Str.data(), len + 1 );
    ipv4[len] = 0;
}

bool viewer_isConnected( const client::Viewer* viewer ) {
    return viewer->isConnected();
}

void sensorSpec_getSensorName( const sensor::SensorSpec* sensorSpec, char* sensorName, int* strLen ) {
    *strLen = sensorSpec->getSensorName().size(); // todo
#if CPLUSPLUS_VERSION == 20
    memcpy( sensorName, sensorSpec->getSensorName().data(), *strLen + 1 );
#else
    memcpy( sensorName, sensorSpec->getSensorName().c_str(), *strLen + 1 );
#endif
    sensorName[*strLen] = 0;
}

int sensorSpec_getResolutionSize( const sensor::SensorSpec* sensorSpec, int iResolution ) {
    return sensor::resolution::computeAcquisitionSize( sensorSpec->getResolutions().at( iResolution ) ); // todo
}

int sensorSpec_getResolutionsSize( const sensor::SensorSpec* sensorSpec ) {
    return sensorSpec->getResolutions().size(); // todo
}

int sensorSpec_getAcquisitionSize( const sensor::SensorSpec* sensorSpec ) {
    return sensorSpec->getAcquisitionSize(); //todo
}

int sensorSpec_getFormat( const sensor::SensorSpec* sensorSpec, int iResolution ) {
    assert( iResolution < sensorSpec->getResolutions().size() );
    return static_cast<int>( sensorSpec->getResolutions().at( iResolution ).second );
}

int sensorSpec_getDimensionsSize( const sensor::SensorSpec* sensorSpec, int iResolution ) {
    assert( iResolution < sensorSpec->getResolutions().size() );
    return sensorSpec->getResolutions().at( iResolution ).first.size(); //todo
}

int sensorSpec_getDimension( const sensor::SensorSpec* sensorSpec, int iResolution, int iDimension ) {
    assert( iResolution < sensorSpec->getResolutions().size() );
    assert( iDimension < sensorSpec->getResolutions().at( iResolution ).first.size() );
    return sensorSpec->getResolutions().at( iResolution ).first.at( iDimension );
}

void sensorSpec_getResolutionsStr( const sensor::SensorSpec* sensorSpec, char* resolutionsStr ) {
    const auto& resolutions = sensorSpec->getResolutions();
    std::stringstream ss;
    ss << resolutions;
    const auto& resolutionsString = ss.str();
    const int len                 = resolutionsString.size(); // todo
    memcpy( resolutionsStr, resolutionsString.c_str(), len + 1 );
    resolutionsStr[len] = 0;
}

void sensorSpec_getMetaDataStr( const sensor::SensorSpec* sensorSpec, char* metaDataStr ) {
    const auto& metaDataString = sensor::SensorSpec::to_string( sensorSpec->getMetaData(), true );
    const int len              = metaDataString.size();  // todo
    memcpy( metaDataStr, metaDataString.c_str(), len + 1 );
    metaDataStr[len] = 0;
}

sensor::SensorSpec* sensorSpec_copy( const sensor::SensorSpec* source ) {
    return new sensor::SensorSpec(
        source->getSensorName(), source->getResolutions(), source->getMetaData() );
}

void freeSensorSpec( sensor::SensorSpec* sensorSpec ) {
    assert( sensorSpec != nullptr );
    std::cout << "[Native] freeSensorSpec( " << sensorSpec << ")" << std::endl;
    delete sensorSpec;
}

const sensor::SensorSpec::MetaData* sensorSpec_getMetaData( const sensor::SensorSpec* sensorSpec ) {
    return &sensorSpec->getMetaData();
}

bool metaData_getString( const sensor::SensorSpec::MetaData* metaData,
                         const char* metaName,
                         char* output,
                         int* strLen ) {
    if ( metaData->find( metaName ) == metaData->end() ) return false;

    const char* meta = metaData->at( metaName ).get<const char *>();
    *strLen          = strlen( meta ); // todo
    memcpy( output, meta, *strLen + 1 );
    output[*strLen] = 0;
    return true;
}

bool metaData_getMat4( const sensor::SensorSpec::MetaData* metaData, const char* metaName, float* output ) {
    if ( metaData->find( metaName ) != metaData->end() ) {
        const auto& mat4 = metaData->at( metaName ).get<data::Mat4>();
        memcpy( output, mat4.getData(), 64 );
        return true;
    }
    return false;
}

bool metaData_exists( const sensor::SensorSpec::MetaData* metaData, const char* metaName ) {
    return metaData->find( metaName ) != metaData->end();
}

int metaData_getInt( const sensor::SensorSpec::MetaData* metaData, const char* metaName ) {
    assert( metaData->find( metaName ) != metaData->end() );
    return metaData->at( metaName ).get<int>();
}

double metaData_getDouble( const sensor::SensorSpec::MetaData* metaData, const char* metaName ) {
    assert( metaData->find( metaName ) != metaData->end() );
    return metaData->at( metaName ).get<double>();
}

 double any_getDouble( const Any* any ) {
     return any->get<double>();
 }

 int any_getInt( const Any* any ) {
     return any->get<int>();
 }

} // namespace native
} // namespace hub
