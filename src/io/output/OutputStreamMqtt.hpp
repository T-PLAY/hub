#pragma once

#include <mutex>
#include <thread>

//#include "OutputStreamInterface.hpp"
//#include "io/output/Output.hpp"
//#include "io/StreamInterface.hpp"
#include "Output.hpp"
#include "io/StreamInterface.hpp"
#include "io/StreamMqtt.hpp"

#include <mqtt/client.h>

namespace hub {
namespace output {


//class SRC_API OutputStreamMqtt : public OutputStreamInterface
class SRC_API OutputStreamMqtt : public io::Output, public io::StreamMqtt
{
  public:
    explicit OutputStreamMqtt( const std::string& streamName,
                                 const std::string & ipv4 = s_defaultIpv4, int port = s_defaultPort);

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
//    std::string m_streamName;

    std::string m_currentTopic;

    uint64_t m_acqSize = 0;
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
    assert(m_msgPtr->get_qos() == 2);
//    assert(m_msgPtr->is_retained());
#ifdef DEBUG_OUTPUT_STREAM
    std::cout << DEBUG_OUTPUT_STREAM + m_name + " write(const unsigned char*, size_t) len = " << len << ", current topic = " << m_currentTopic << std::endl;
#endif
    m_msgPtr->set_payload((char*)data, len);
//    m_msgPtr->set_qos( 2 );
//    m_msgPtr->set_retained(true);
    m_client->publish(m_msgPtr);
//    assert(m_client->is_connected());
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
    m_msgPtr->set_payload(io::StreamMqtt::to_string(StreamMqtt::Message::DEL_STREAM) + m_name);
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
