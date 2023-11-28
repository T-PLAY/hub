
#pragma once

#include <thread>

#include <mqtt/client.h>

// #include "InputStreamInterface.hpp"
#include "core/Input.hpp"
//#include "io/input/"
// #include "impl/mqtt/io/StreamMqtt.hpp"
#include "io/StreamMqtt.hpp"
#include "sensor/Acquisition.hpp"
#include "sensor/SensorSpec.hpp"

namespace hub {
namespace input {

// class SRC_API InputStreamMqtt : public InputStreamInterface
class SRC_API InputStreamMqtt : public Input, public io::StreamMqtt
{
  public:
    explicit InputStreamMqtt( const std::string& streamName,
                              const std::string& ipv4 = s_defaultIpv4,
                              int port                = s_defaultPort );

    //        InputStreamMqtt( InputStreamMqtt&& inputStream );

    //        ~InputStreamMqtt();

  protected:
  public:
    bool isOpen() const override;

    void read( unsigned char* data, size_t len ) override;

    void close() override;

    bool isEnd() const override;

    void read( sensor::Acquisition& acq );
    void read( sensor::SensorSpec& sensorSpec );

  private:
    void subscribe(const std::string & topic);

    std::unique_ptr<mqtt::client> m_client;
    mqtt::const_message_ptr m_msgPtr;

    std::string m_currentTopic;
    //    mqtt::message_ptr m_msgPtr;

    //    friend class InputSyncStream;
    uint64_t m_acqSize = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool InputStreamMqtt::isOpen() const {
    return m_client->is_connected();
}

inline void InputStreamMqtt::read( unsigned char* data, size_t len ) {
#ifdef HUB_DEBUG_INPUT
    std::cout << HEADER_INPUT_MSG "read(data, len = " << len << ")" << std::endl;
#endif
#ifdef DEBUG_INPUT_STREAM
    std::cout << DEBUG_INPUT_STREAM "read(data, len = " << len << ")" << std::endl;
#endif

    assert( m_client->is_connected() );
    //    m_client->start_consuming();
//    do {
//            bool consumed = m_client->try_consume_message(&m_msgPtr);
        m_msgPtr.reset();
        const bool consumed =
            m_client->try_consume_message_for( &m_msgPtr, std::chrono::milliseconds( 1000 ) );
//        m_msgPtr = m_client->consume_message();
//        bool consumed = m_msgPtr != nullptr;
        if ( !consumed ) {
            throw io::StreamMqtt::exception(
                ("[InputStreamMqtt] read(unsigned char*, size_t) current topic : '" + m_currentTopic + "' unable to receive data from stream").c_str() );
        }
        assert( consumed );
//        if ( m_msgPtr->get_topic() != m_currentTopic ) {

//            std::cout << "[InputStreamMqtt] msg topic : '" + m_msgPtr->get_topic() +
//                             ", current topic : '" + m_currentTopic
//                      << "', waiting for new msg" << std::endl;
//        }
//    } while ( m_msgPtr->get_topic() == m_currentTopic );

    //    m_msgPtr.reset();
    //    m_msgPtr = m_client->consume_message();
    //    m_client->stop_consuming();
    assert( m_msgPtr != nullptr );

    const auto& payload = m_msgPtr->get_payload();
#ifdef DEBUG_INPUT_STREAM
    std::cout << DEBUG_INPUT_STREAM + m_name + " read(unsigned char *, size_t)) current topic : '"
              << m_currentTopic << "', msg topic : '" << m_msgPtr->get_topic() << "', reading " << len
              << " bytes and payload size = " << payload.length() << std::endl;
#endif
    assert(m_msgPtr->get_qos() == 1);
//    assert(m_msgPtr->is_retained());

    //    assert(m_msgPtr->get_topic() == m_currentTopic);

    if ( payload.length() != len || m_msgPtr->get_topic() != m_currentTopic ) {

#ifdef DEBUG_INPUT_STREAM
        std::cout << DEBUG_INPUT_STREAM + m_name + " read(unsigned char *, size_t)) msg topic is "
                  << m_msgPtr->get_topic() << std::endl;
#endif
        //        std::cerr << "[InputStreamMqtt] payload is corrupted : '" << payload << "', topic
        //        : '" << m_currentTopic << "'" << std::endl;
        throw io::StreamMqtt::exception(
            ( std::string( "[InputStreamMqtt] msg topic is '" + m_msgPtr->get_topic() +
                           ", payload is corrupted topic : '" ) +
              m_currentTopic + "', data : '" + payload + "'" )
                .c_str() );
    }
    assert( payload.length() == len );
    memcpy( data, payload.data(), len );
    //    assert(m_msgPtr != nullptr);
    //    m_msgPtr->set_payload((char*)data, len);
    //    m_client->publish(m_msgPtr);
    //    assert(m_client->is_connected());
}

inline void InputStreamMqtt::close() {
    assert( m_client->is_connected() );
    m_client->disconnect();
    assert( !m_client->is_connected() );
}

inline bool InputStreamMqtt::isEnd() const {
    //    return ! m_client->is_connected();
    //    m_client->start_consuming();
    //    bool consumed = m_client->try_consume_message(&m_msgPtr);
    ////    assert(consumed);
    ////    m_msgPtr = m_client->consume_message();
    //    m_client->stop_consuming();
    //    return ! consumed;
    return false;
}

} // namespace input
} // namespace hub
