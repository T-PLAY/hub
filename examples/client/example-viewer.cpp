
#include <client/Viewer.hpp>
#include <sensor/OutputSensor.hpp>

int main() {
    const std::string ipv4 = "127.0.0.1";
    const int port         = 1883;

    hub::client::ViewerHandler viewerHandler;
    // startConstruction
    viewerHandler.onNewStream = [=]( const std::string& streamName,
                                     // const hub::sensor::SensorSpec& sensorSpec ) {
                                     const hub::io::Header& header ) {
        std::cout << "\t[example-viewer] onNewStream : " << streamName << std::endl;
        // accept all stream to run
        return true;
    };
    viewerHandler.onDelStream = []( const std::string& streamName )
    // ,
    // const hub::sensor::SensorSpec& sensorSpec )
    { std::cout << "\t[example-viewer] onDelStream : " << streamName << std::endl; };
    viewerHandler.onServerNotFound = []( const std::string& ipv4, int port ) {
        std::cout << "\t[example-viewer] onServerNotFound : " << ipv4 << " " << port << std::endl;
    };
    viewerHandler.onServerConnected = []( const std::string& ipv4, int port ) {
        std::cout << "\t[example-viewer] onServerConnected : " << ipv4 << " " << port << std::endl;
    };
    viewerHandler.onServerDisconnected = []( const std::string& ipv4, int port ) {
        std::cout << "\t[example-viewer] onServerDisconnected : " << ipv4 << " " << port
                  << std::endl;
    };
    viewerHandler.onNewData = []( const std::string& streamName, const hub::Datas_t& datas ) {
        std::cout << "\t[example-viewer] onNewData : " << datas << std::endl;
    };
    // viewerHandler.onNewAcquisition = []( const std::string& streamName,
    //                             const hub::sensor::Acquisition& acq ) {
    //     std::cout << "\t[example-viewer] onNewAcquisition : " << acq << std::endl;
    // };
    viewerHandler.onSetProperty = []( const std::string& streamName,
                                      const std::string& objectName,
                                      int property,
                                      const hub::Any& value ) {
        std::cout << "\t[example-viewer] onSetProperty " << streamName << std::endl;
    };
    viewerHandler.onLogMessage = []( const std::string& logMessage ) {
        std::cout << "[example-viewer] onLogMessage '" << logMessage << "'" << std::endl;
    };

    hub::client::Viewer viewer( FILE_NAME, std::move( viewerHandler ), ipv4, port );
}
