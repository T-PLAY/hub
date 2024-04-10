
#include "native_ViewerHandler.hpp"

#include "client/viewer/ViewerHandler.hpp"

namespace hub {
namespace native {

client::ViewerHandler* createViewerHandler( onServerNotFoundFunc onServerNotFound,
                                            onServerConnectedFunc onServerConnected,
                                            onServerDisconnectedFunc onServerDisconnected,
                                            onNewStreamFunc onNewStream,
#ifndef HUB_NON_BUILD_SENSOR
                                            onNewSensorFunc onNewSensor,
#endif
                                            onNewDataFunc onNewData,
#ifndef HUB_NON_BUILD_SENSOR
                                            onNewAcqFunc onNewAcq,
#endif
                                            onDelStreamFunc onDelStream,
                                            onSetPropertyFunc onSetProperty,
                                            onLogMessageFunc onLogMessage ) {

    client::ViewerHandler* viewerHandler = new client::ViewerHandler;

    viewerHandler->onServerNotFound = [=]( const std::string& ipv4, int port ) {
        onServerNotFound( ipv4.c_str(), port );
    };
    viewerHandler->onServerConnected = [=]( const std::string& ipv4, int port ) {
        onServerConnected( ipv4.c_str(), port );
    };
    viewerHandler->onServerDisconnected = [=]( const std::string& ipv4, int port ) {
        onServerDisconnected( ipv4.c_str(), port );
    };
    viewerHandler->onNewStream = [=]( const std::string& streamName, const io::Header& header ) {
        return onNewStream( streamName.c_str(), &header );
    };
#ifndef HUB_NON_BUILD_SENSOR
    viewerHandler->onNewSensor = [=]( const std::string& streamName,
                                      const sensor::SensorSpec& sensorSpec ) {
        return onNewSensor( streamName.c_str(), &sensorSpec );
    };
#endif
    viewerHandler->onNewData = [=]( const std::string& streamName, const hub::Datas_t& datas ) {
        onNewData( streamName.c_str(), &datas );
    };
#ifndef HUB_NON_BUILD_SENSOR
    viewerHandler->onNewAcq = [=]( const std::string& streamName, const sensor::Acquisition& acq ) {
        onNewAcq( streamName.c_str(), &acq );
    };
#endif
    viewerHandler->onDelStream = [=]( const std::string& streamName ) {
        onDelStream( streamName.c_str() );
    };
    viewerHandler->onSetProperty = [=]( const std::string& streamName,
                                        const std::string& objectName,
                                        int property,
                                        const Any& value ) {
        onSetProperty( streamName.c_str(), objectName.c_str(), property, &value );
    };
    viewerHandler->onLogMessage = [=]( const std::string& logMessage ) {
        onLogMessage( logMessage.c_str() );
    };
    return viewerHandler;
}

void freeViewerHandler( client::ViewerHandler* viewerHandler ) {
    assert( viewerHandler != nullptr );
    delete viewerHandler;
}

int printStatus() {
    return 4;
}

} // namespace native
} // namespace hub
