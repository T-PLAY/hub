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

//    void write( const Acquisition& acq ) override;

  protected:
    void write( const unsigned char* data, size_t len ) override;
    void close() override;
    bool isOpen() const override;


  private:
    std::unique_ptr<mqtt::client> m_client;
    mqtt::message_ptr m_msgPtr;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//inline void OutputStreamMqtt::write( const Acquisition& acq ) {
//}

inline void OutputStreamMqtt::write( const unsigned char* data, size_t len ) {
    assert(m_client->is_connected());
    assert(m_msgPtr != nullptr);
    m_msgPtr->set_payload((char*)data, len);
    m_client->publish(m_msgPtr);
    assert(m_client->is_connected());
}

inline void OutputStreamMqtt::close() {
    assert(m_client->is_connected());
    m_client->disconnect();
    assert(! m_client->is_connected());
}

inline bool OutputStreamMqtt::isOpen() const {
    return m_client->is_connected();
}

} // namespace output
} // namespace hub
