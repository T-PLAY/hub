
#include "native_ViewerHandler.hpp"

namespace hub {
namespace native {

// template <class InputStream>
client::ViewerHandler* createViewerHandler( onServerNotFoundFunc onServerNotFound,
                                            onServerConnectedFunc onServerConnected,
                                            onServerDisconnectedFunc onServerDisconnected,
                                            onNewStreamerFunc onNewStreamer,
                                            onDelStreamerFunc onDelStreamer,
                                            onNewAcquisitionFunc onNewAcquisition,
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
    viewerHandler->onNewStreamer = [=]( const std::string& streamName,
                                        const sensor::SensorSpec& sensorSpec ) {
        return onNewStreamer( streamName.c_str(), &sensorSpec );
    };
    viewerHandler->onDelStreamer = [=]( const std::string& streamName,
                                        const sensor::SensorSpec& sensorSpec ) {
        onDelStreamer( streamName.c_str(), &sensorSpec );
    };
    viewerHandler->onNewAcquisition = [=]( const std::string& streamName,
                                           const sensor::Acquisition& acq ) {
        onNewAcquisition( streamName.c_str(), &acq );
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
    // client::ViewerHandler* viewerHandler = new client::ViewerHandler( onServerNotFoundCpp,
    //                                                            onServerConnectedCpp,
    //                                                            onServerDisconnectedCpp,
    //                                                            onNewStreamerCpp,
    //                                                            onDelStreamerCpp,
    //                                                            onNewAcquisitionCpp,
    //                                                            onSetPropertyCpp,
    //                                                            onLogMessageCpp );
    return viewerHandler;
}

void freeViewer( client::ViewerHandler* viewerHandler ) {
    assert( viewerHandler != nullptr );
    delete viewerHandler;
}

} // namespace native
} // namespace hub
