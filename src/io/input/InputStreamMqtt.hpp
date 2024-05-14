/*******************************************************************************
* Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
* Please visit https://www.irit.fr/tplay/.
*
* All rights reserved.
* This code belongs to tplay/hub project (https://github.com/T-PLAY/hub).
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*
* Initial Contributors:
*   - Gauthier Bouyjou
*******************************************************************************/

#pragma once

#ifdef HUB_USE_MQTT

#    include <thread>

#    include <mqtt/client.h>

#    include "core/Input.hpp"
#    include "io/StreamMqtt.hpp"
#    include "sensor/Acquisition.hpp"
#    include "sensor/SensorSpec.hpp"

namespace hub {
namespace input {

///
/// \brief The InputStreamMqtt class
///
class SRC_API InputStreamMqtt : public Input, public io::StreamMqtt
{
  public:
    explicit InputStreamMqtt( const std::string& streamName,
                              int port                = s_defaultPort,
                              const std::string& ipv4 = s_defaultIpv4 );

  protected:
  public:
    bool isOpen() const override;

    void read( unsigned char* data, size_t len ) override;

    void close() override;

    bool isEnd() const override;
    void clear() override;

    void read( sensor::Acquisition& acq );
    void read( sensor::SensorSpec& sensorSpec );

  private:
    void subscribe( const std::string& topic );

    std::unique_ptr<mqtt::client> m_client;
    mqtt::const_message_ptr m_msgPtr;

    std::string m_currentTopic;

    uint64_t m_acqSize = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool InputStreamMqtt::isOpen() const {
    return m_client->is_connected();
}

inline void InputStreamMqtt::read( unsigned char* data, size_t len ) {
#    ifdef HUB_DEBUG_INPUT
    std::cout << "[InputStreamMqtt] read(data, len = " << len << ")" << std::endl;
#    endif
#    ifdef DEBUG_INPUT_STREAM
    std::cout << DEBUG_INPUT_STREAM + "read(data, len = " << len << ")" << std::endl;
#    endif

    assert( m_client->is_connected() );
    m_msgPtr.reset();
    const bool consumed =
        m_client->try_consume_message_for( &m_msgPtr, std::chrono::milliseconds( 1000 ) );
    if ( !consumed ) {
        throw io::StreamMqtt::exception(
            ( "[InputStreamMqtt] read(unsigned char*, size_t) current topic : '" + m_currentTopic +
              "' unable to receive data from stream" )
                .c_str() );
    }
    assert( consumed );

    assert( m_msgPtr != nullptr );

    const auto& payload = m_msgPtr->get_payload();
#    ifdef DEBUG_INPUT_STREAM
    std::cout << DEBUG_INPUT_STREAM + m_name + " read(unsigned char *, size_t)) current topic : '"
              << m_currentTopic << "', msg topic : '" << m_msgPtr->get_topic() << "', reading "
              << len << " bytes and payload size = " << payload.length() << std::endl;
#    endif
    assert( m_msgPtr->get_qos() == 1 );

    if ( payload.length() != len || m_msgPtr->get_topic() != m_currentTopic ) {

#    ifdef DEBUG_INPUT_STREAM
        std::cout << DEBUG_INPUT_STREAM + m_name + " read(unsigned char *, size_t)) msg topic is "
                  << m_msgPtr->get_topic() << std::endl;
#    endif
        throw io::StreamMqtt::exception(
            ( std::string( "[InputStreamMqtt] msg topic is '" + m_msgPtr->get_topic() +
                           ", payload is corrupted topic : '" ) +
              m_currentTopic + "', data : '" + payload + "'" )
                .c_str() );
    }
    assert( payload.length() == len );
    memcpy( data, payload.data(), len );
}

inline void InputStreamMqtt::close() {
    assert( m_client->is_connected() );
    m_client->disconnect();
    assert( !m_client->is_connected() );
}

inline bool InputStreamMqtt::isEnd() const {
    ////    assert(consumed);
    ////    m_msgPtr = m_client->consume_message();
    return false;
}

} // namespace input
} // namespace hub

#endif
