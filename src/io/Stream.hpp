#pragma once

#include <exception>
#include <functional>
#include <iostream>
#include <string>

namespace hub {
namespace io {

const std::string s_topicStream = "streams/";
const std::string s_topicViewer = "viewers/";
const std::string s_topicEvents = "events";

const std::string s_mqttDefaultIpv4 = "127.0.0.1";
constexpr int s_mqttDefaultPort = 1883;

const std::string s_serverDefaultIpv4 = "127.0.0.1";
constexpr int s_serverDefaultPort = 4042;

class Stream
{
  public:
    class exception : public std::runtime_error
    {
      public:
        ///
        /// \brief exception
        /// launched when peer connection is lost or invalid.
        /// \param message
        /// shown by the client application when exception has been launched.
        ///
        explicit exception( const char* const message ) throw() : std::runtime_error( message ) {}

        ///
        /// \brief what
        /// allow the user to catch message exception.
        /// \return
        /// message describe the exception.
        ///
        const char* what() const throw() { return std::runtime_error::what(); }
    };
    enum class Message {
        NONE = 0,
        NEW_STREAM,
        DEL_STREAM,
        SET_PROPERTY,
        COUNT
    };
    static std::string to_string(const Message & message);

  protected:
    Stream( const std::string& name, const std::string& ipv4, int port );
    Stream( const char * name, const char * ipv4, int port );

  protected:
    const std::string m_name;
    const std::string m_ipv4;
    const int m_port;
};

} // namespace io
} // namespace hub
