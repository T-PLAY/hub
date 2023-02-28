#include "Asker.hpp"

//#include <regex>
//#include <sstream>

//#include "InputSensor.hpp"


namespace hub {

Asker::Asker(
//    std::function<bool( const char* streamName, const SensorSpec& )> onNewStreamer,
//    std::function<void( const char* streamName, const SensorSpec& )> onDelStreamer,
//    std::function<void( const char* ipv4, int port )> onServerConnected,
//    std::function<void( const char* ipv4, int port )> onServerDisconnected,
//    std::function<void( const char* streamName, const hub::Acquisition& )> onNewAcquisition,
    const std::string& ipv4,
    int port
//    bool autoSync,
//    std::function<void( const char* logMessage )> onLogMessage ) :
       ) :

//    m_onNewStreamer( onNewStreamer ),
//    m_onDelStreamer( onDelStreamer ),
//    m_onServerConnected( onServerConnected ),
//    m_onServerDisconnected( onServerDisconnected ),
//    m_onNewAcquisition( onNewAcquisition ),
    m_sock( ipv4, port )
//    m_autoSync( autoSync ),
//    m_onLogMessage( onLogMessage )
{

//    m_thread = std::thread( [this]() {
//        while ( !m_stopThread ) {
//            try {

//                assert( !m_sock.isOpen() );
//                m_sock.connect();
//                m_serverConnected = true;
                assert( m_sock.isOpen() );

                m_sock.write( net::ClientSocket::Type::ASKER );

//                if ( m_onServerConnected )
//                    m_onServerConnected( m_sock.getIpv4().c_str(), m_sock.getPort() );

//                while ( !m_stopThread ) {
//                    case net::ClientSocket::Message::PING: {
//                    } break;

//                    case net::ClientSocket::Message::NEW_STREAMER: {
//            }

//        } // while (! m_stopThread)
//    } );  // thread

}

Asker::~Asker()
{
    m_sock.write( net::ClientSocket::Message::CLOSE );

}

std::list<std::pair<std::string, SensorSpec> > Asker::listStreams() const
{
        m_sock.write( net::ClientSocket::Message::LIST_STREAMS );

//        std::list<std::string> streams;
//        m_sock.read(streams);

//        std::vector<hub::SensorSpec> streamSpecs;
//        for (int i = 0; i <streams.size(); ++i) {
////        m_sock.read(streamSpecs);
//            auto && sensorSpec = m_sock.getSensorSpec();
////std::cout << sensorSpec << std::endl;
//            streamSpecs.push_back(std::move(sensorSpec));
//        }

        std::list<std::pair<std::string, hub::SensorSpec>> ret;
        m_sock.read(ret);

//        int i = 0;
//        for (const )


        return ret;
}

SensorSpec Asker::getSensorSpec(const std::string &streamName) const
{
        m_sock.write( net::ClientSocket::Message::GET_SENSOR_SPEC );
        m_sock.write( streamName);

        net::ClientSocket::Message message;
        m_sock.read(message);

        if (message == net::ClientSocket::Message::NOT_FOUND) {
            throw net::ClientSocket::exception(
            ( std::string( "stream '" ) + streamName + "' is not attached to server" ).c_str() );
        }
        assert(message == net::ClientSocket::Message::FOUND);

//        hub::SensorSpec ret;
//        m_sock.read(ret);
        auto sensorSpec = m_sock.getSensorSpec();
        return sensorSpec;
}

Acquisition Asker::getAcquisition(const std::string &streamName) const
{
        m_sock.write( net::ClientSocket::Message::GET_ACQUISITION );
        m_sock.write( streamName);

        net::ClientSocket::Message message;
        m_sock.read(message);

        if (message == net::ClientSocket::Message::NOT_FOUND) {
            throw net::ClientSocket::exception(
            ( std::string( "stream '" ) + streamName + "' is not attached to server" ).c_str() );
        }
        assert(message == net::ClientSocket::Message::FOUND);

        auto sensorSpec = m_sock.getSensorSpec();

        auto acq = m_sock.getAcquisition(sensorSpec);

        return acq;
}

} // namespace hub
