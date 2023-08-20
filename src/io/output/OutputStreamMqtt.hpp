#pragma once

#include <mutex>
#include <thread>

#include "OutputStreamInterface.hpp"

#include <mqtt/client.h>

namespace hub {
using namespace io;
namespace output {


class SRC_API OutputStreamMqtt : public OutputStreamInterface
{
  public:
    explicit OutputStreamMqtt( const std::string& streamName,
                                 const std::string & ipv4, int port);

//    OutputStreamMqtt( OutputStreamMqtt&& outputStream );

//    ~OutputStreamMqtt();

    void write( const Acquisition& acq ) override;
    void write( const SensorSpec& sensorSpec ) override;
//    void write(uint64_t packetSize);

  protected:
    void write( const unsigned char* data, size_t len ) override;
    void close() override;
    bool isOpen() const override;


  private:
    std::unique_ptr<mqtt::client> m_client;
    mqtt::message_ptr m_msgPtr;
    std::string m_streamName;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//inline void OutputStreamMqtt::write( const Acquisition& acq ) {
//}

inline void OutputStreamMqtt::write( const unsigned char* data, size_t len ) {
#ifdef DEBUG_OUTPUT
    std::cout << HEADER_OUTPUT_MSG "write(data, len = " << len << ")" << std::endl;
#endif
    assert(m_client->is_connected());
    assert(m_msgPtr != nullptr);
    m_msgPtr->set_payload((char*)data, len);
//    m_msgPtr->set_retained(true);
    m_client->publish(m_msgPtr);
    assert(m_client->is_connected());
}

inline void OutputStreamMqtt::close() {
    assert(m_client->is_connected());

//    m_msgPtr->set_retained(false);
//    m_msgPtr->set_qos(2);
    m_msgPtr->set_payload("none");
//    m_msgPtr->set_topic(m_name + "/#");
//    m_client->publish(m_msgPtr);

    m_msgPtr->set_topic(s_topicStream + m_name + "/header/size");
//    m_msgPtr->clear_payload();
//    m_msgPtr->set_payload("none");
    m_client->publish(m_msgPtr);

    m_msgPtr->set_topic(s_topicStream + m_name + "/header/data");
//    m_msgPtr->clear_payload();
//    m_msgPtr->set_payload("none");
    m_client->publish(m_msgPtr);

    m_msgPtr->set_topic(s_topicStream + m_name + "/acq/size");
//    m_msgPtr->clear_payload();
//    m_msgPtr->set_payload("none");
    m_client->publish(m_msgPtr);

    m_msgPtr->set_topic(s_topicStream + m_name + "/acq/data");
//    m_msgPtr->clear_payload();
//    m_msgPtr->set_payload("none");
    m_client->publish(m_msgPtr);

    m_msgPtr->set_payload("inactive");
    m_msgPtr->set_topic(s_topicStream + m_name);
    m_client->publish(m_msgPtr);

    // prevent viewers there is streamer done
    m_msgPtr->set_retained(false);
    m_msgPtr->set_topic(s_topicEvents);
    m_msgPtr->set_payload(Stream::to_string(Stream::Message::DEL_STREAM) + m_streamName);
//    m_msgPtr->set_payload("del sensor");
    m_client->publish(m_msgPtr);

//    m_msgPtr->set_topic(s_topicEvents + "/streamName");
//    m_msgPtr->set_payload(m_streamName);
//    m_client->publish(m_msgPtr);

    m_client->disconnect();
    assert(! m_client->is_connected());
}

inline bool OutputStreamMqtt::isOpen() const {
    return m_client->is_connected();
}

} // namespace output
} // namespace hub
