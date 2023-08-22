
#pragma once

#include <thread>

#include <mqtt/client.h>

// #include "InputStreamInterface.hpp"
#include "Input.hpp"
#include "io/StreamMqtt.hpp"

namespace hub {
namespace input {

// class SRC_API InputStreamMqtt : public InputStreamInterface
class SRC_API InputStreamMqtt : public io::Input, public io::StreamMqtt
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

    void read( Acquisition& acq ) override;
    void read( SensorSpec& sensorSpec ) override;

  private:
    std::unique_ptr<mqtt::client> m_client;
    mqtt::const_message_ptr m_msgPtr;

    std::string m_currentTopic;
    //    mqtt::message_ptr m_msgPtr;

    //    friend class InputSyncStream;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool InputStreamMqtt::isOpen() const {
    return m_client->is_connected();
}

inline void InputStreamMqtt::read( unsigned char* data, size_t len ) {
#ifdef DEBUG_INPUT
    std::cout << HEADER_INPUT_MSG "read(data, len = " << len << ")" << std::endl;
#endif
    assert( m_client->is_connected() );
    m_client->start_consuming();
    //    bool consumed = m_client->try_consume_message(&m_msgPtr);
    //    bool consumed = m_client->try_consume_message_for(&m_msgPtr,
    //    std::chrono::milliseconds(100)); assert(consumed);
    m_msgPtr = m_client->consume_message();
//    assert(m_msgPtr->get_qos() == 2);
//    assert(m_msgPtr->is_retained());
    m_client->stop_consuming();
    assert( m_msgPtr != nullptr );
    const auto& payload = m_msgPtr->get_payload();
    if ( payload.length() != len ) {
        //        std::cerr << "[InputStreamMqtt] payload is corrupted : '" << payload << "', topic
        //        : '" << m_currentTopic << "'" << std::endl;
        throw io::StreamMqtt::exception(
            ( std::string( "[InputStreamMqtt] payload is corrupted topic : '" ) + m_currentTopic +
              "', data : '" + payload + "'" )
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
