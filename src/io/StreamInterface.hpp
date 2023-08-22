#pragma once

#include <string>
#include <iostream>

namespace hub {
namespace io {


class StreamInterface
{
  public:
    static const std::string s_defaultIpv4;
    static const int s_defaultPort;

    enum class Message {
        NONE = 0,
        NEW_STREAM,
        DEL_STREAM,
        SET_PROPERTY,
        COUNT
    };
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


  protected:
    StreamInterface( const std::string& name, const std::string& ipv4, int port );
    StreamInterface( const char * name, const char * ipv4, int port );

    static std::string to_string(const Message & message);
//    static const std::string getDefaultIpv4();
//    static constexpr int getDefaultPort();



    const std::string m_name;
    const std::string m_ipv4;
    const int m_port;
};



} // namespace io
} // namespace hub
