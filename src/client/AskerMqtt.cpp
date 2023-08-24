#include "AskerMqtt.hpp"

#include "utils/Utils.hpp"

#include "io/Memory.hpp"

namespace hub {
namespace client {

AskerMqtt::AskerMqtt( const std::string& ipv4, int port ) :
    m_hostName( hub::utils::getHostname() ),
    m_client( new mqtt::client( ipv4 + ":" + std::to_string( port ),
                                std::string( "asker" ) + m_hostName,
                                mqtt::create_options( MQTTVERSION_5 ) ) ) {

    m_client->connect();
    assert( m_client->is_connected() );
//        m_client->start_consuming();
    //    m_sock( ipv4, port ) {
    //    assert( m_sock.isOpen() );
    //    m_sock.write( net::ClientSocket::Type::ASKER );
}

AskerMqtt::~AskerMqtt() {
    //    m_sock.write( net::ClientSocket::Message::CLOSE );
    //    assert( m_sock.isOpen() );
    //    m_sock.close();
    //    assert( !m_sock.isOpen() );
}

std::list<std::pair<std::string, SensorSpec>> AskerMqtt::listStreams() {
    //    m_sock.write( net::ClientSocket::Message::LIST_STREAMS );

    assert(m_client->is_connected());
    m_client->start_consuming();

    //    ////std::cout << sensorSpec << std::endl;
    m_client->subscribe( io::StreamMqtt::s_topicStream + "+" );

    std::vector<std::string> activeStreamTopics;

    while ( true ) {
        m_inputMsgPtr.reset();
        //    m_inputMsgPtr = m_client->consume_message();
        m_client->try_consume_message_for( &m_inputMsgPtr, std::chrono::milliseconds( 100 ) );
        if ( m_inputMsgPtr == nullptr ) break;
        assert( m_inputMsgPtr != nullptr );

        std::cout << "[AskerMqtt] input msg : " << m_inputMsgPtr->get_payload_str() << std::endl;
        std::cout << "[AskerMqtt] input msg topic : " << m_inputMsgPtr->get_topic() << std::endl;
        if ( m_inputMsgPtr->get_payload_str() == "active" ) {
            activeStreamTopics.push_back( m_inputMsgPtr->get_topic() );
        }
    }

    std::list<std::pair<std::string, SensorSpec>> ret;

    for ( const auto& activeStreamTopic : activeStreamTopics ) {

        uint64_t packetSize;
        m_client->subscribe( activeStreamTopic + "/header/size" );
        //    m_client->subscribe( m_currentTopic );
//        Input::read( packetSize );
        m_inputMsgPtr.reset();
        m_inputMsgPtr = m_client->consume_message();
        assert(m_inputMsgPtr != nullptr);
        assert(m_inputMsgPtr->get_topic() == activeStreamTopic + "/header/size");
        //    m_client->unsubscribe( m_currentTopic );
//        memcpy(&packetSize, m_inputMsgPtr->get_payload().data(), sizeof(uint64_t));
        read(packetSize);


        std::vector<char> buff( sizeof( uint64_t ) + packetSize );
        //    unsigned char data[sizeof(uint64_t) + packetSize];
        auto* data             = buff.data();
        ( (uint64_t*)data )[0] = packetSize;

        m_client->subscribe( activeStreamTopic + "/header/data" );
        //    m_client->subscribe( m_currentTopic );
//        read( (unsigned char*)&data[sizeof( uint64_t )], packetSize );
        //    m_client->unsubscribe( m_currentTopic );
        m_inputMsgPtr.reset();
        m_inputMsgPtr = m_client->consume_message();
        assert(m_inputMsgPtr != nullptr);
        assert(m_inputMsgPtr->get_topic() == activeStreamTopic + "/header/data");
        memcpy(&data[sizeof(uint64_t)], m_inputMsgPtr->get_payload().data(), packetSize);

        io::Memory<decltype( buff )> memory( buff );
        hub::SensorSpec sensorSpec;
        memory.read( sensorSpec );
        assert( buff.empty() );
        assert( memory.isEnd() );

        std::cout << "[AskerMqtt] sensorSpec : " << sensorSpec << std::endl;
        auto streamName = activeStreamTopic.substr(activeStreamTopic.find('/') + 1);
        ret.push_back(std::make_pair(streamName, sensorSpec));
    }

    m_client->stop_consuming();
//    std::list<std::pair<std::string, hub::SensorSpec>> ret;
    //    m_sock.read( ret );

    return ret;
}

Acquisition AskerMqtt::getAcquisition( const std::string& streamName ) {
    //    m_sock.write( net::ClientSocket::Message::GET_ACQUISITION );
    //    m_sock.write( streamName );
    assert(m_client->is_connected());
    m_client->start_consuming();

        m_inputMsgPtr.reset();
    m_client->subscribe( io::StreamMqtt::s_topicStream + streamName );
        //    m_inputMsgPtr = m_client->consume_message();
        m_client->try_consume_message_for( &m_inputMsgPtr, std::chrono::milliseconds( 100 ) );

    if (m_inputMsgPtr == nullptr || m_inputMsgPtr->get_payload_str() == "inactive") {

            throw io::StreamMqtt::exception(
            ("[Asker] Stream '" + streamName + "' is not connected to server").c_str());
        }

    assert(m_inputMsgPtr->get_payload_str() == "active");


    //    net::ClientSocket::Message message;
    //    m_sock.read( message );

    //    if ( message == net::ClientSocket::Message::NOT_FOUND ) {
    //        throw net::ClientSocket::exception(
    //            ( std::string( "stream '" ) + streamName + "' is not attached to server" ).c_str()
    //            );
    //    }
    //    assert( message == net::ClientSocket::Message::FOUND );

    //    hub::SensorSpec sensorSpec;
    //    m_sock.read( sensorSpec );
    uint64_t acqSize;

    m_client->subscribe(io::StreamMqtt::s_topicStream + streamName + "/acqs/size");
        m_inputMsgPtr.reset();
        m_inputMsgPtr = m_client->consume_message();
        assert(m_inputMsgPtr != nullptr);
        assert(m_inputMsgPtr->get_topic() == io::StreamMqtt::s_topicStream + streamName + "/acqs/size");
    read(acqSize);
    std::cout << "[Asker] acqSize = " << acqSize << std::endl;
//        m_client->subscribe( m_currentTopic );
//        Input::read( acqSize );
        //    read(packetSize);
//        m_client->unsubscribe( m_currentTopic );

//        for ( int i = 0; i < s_bufferAcqSize; ++i ) {
//            m_client->subscribe( s_topicStream + m_name + "/acqs/data" + std::to_string( i ) );
//        }
//        m_client->subscribe( m_currentTopic );
//    }

    std::vector<char> buff( acqSize );
    //    auto * data = buff.data();
    //    ((uint64_t*)data)[0] = m_acqSize;
//    m_currentTopic = s_topicStream + m_name + "/acqs/data" + std::to_string( m_iAcq );
//    m_iAcq         = ( m_iAcq + 1 ) % s_bufferAcqSize;


        m_client->subscribe(io::StreamMqtt::s_topicStream + streamName + "/acqs/data0");
        m_inputMsgPtr.reset();
        m_inputMsgPtr = m_client->consume_message();
        assert(m_inputMsgPtr != nullptr);
        assert(m_inputMsgPtr->get_topic() == io::StreamMqtt::s_topicStream + streamName + "/acqs/data0");
//        m_client->subscribe( m_currentTopic );
    //    read((unsigned char*)&data[sizeof(uint64_t)], m_acqSize);
//    read( (unsigned char*)buff.data(), acqSize );
//    memcpy(buff.data(),
        memcpy(buff.data(), m_inputMsgPtr->get_payload().data(), acqSize);
//        m_client->unsubscribe( m_currentTopic );

    io::Memory<decltype( buff )> memory( buff );
    hub::Acquisition acq;
    memory.read( acq );

    std::cout << "read acq : " << acq << std::endl;
//    std::cout << "\tread acq" << m_iAcq - 1 << " : " << acq << std::endl;

    assert( buff.empty() );
    assert( memory.isEnd() );

    m_client->stop_consuming();

    //    m_sock.read( acq );
    return acq;
}

} // namespace client
} // namespace hub
