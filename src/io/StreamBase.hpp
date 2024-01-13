/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/27
	
#pragma once

#include <iostream>
#include <string>
// #include <ostream>

#include "core/Macros.hpp"
#include "core/Traits.hpp"

#include "Header.hpp"

namespace hub {
namespace io {

// #define DEBUG_INPUT_STREAM ">> \033[" << std::to_string(31 + (long)this % 7) << "m[InputStream:"
// << this << "]\033[0m " #define DEBUG_OUTPUT_STREAM "\t<< \033[" << std::to_string(31 + (long)this
// % 7) << "m[OutputStream:" << this << "]\033[0m "
// struct Header {
//     auto getSize() const {
//         return sizeof( m_headerSize ) + sizeof( m_dataSize ) + m_userData.size();
//     }

//     Header( Size_t dataSize = 0, const Datas_t& headerData = {} ) :
//         m_dataSize( dataSize ), m_userData( headerData ) {
//         m_headerSize = getSize();
//     }

//     bool empty() const { return m_userData.empty(); }
//     bool operator==( const Header& other ) const {
//         return m_headerSize == other.m_headerSize && m_dataSize == other.m_dataSize &&
//                m_userData == other.m_userData;
//     }
//     auto toString() const {
//         return std::to_string( m_headerSize ) + ":" + std::to_string( m_dataSize ) + ":" +
//                hub::toString( m_userData );
//     }
//     static constexpr auto serialize( auto& archive, auto& self ) {
//         return archive( self.m_headerSize, self.m_dataSize, self.m_userData );
//     }

//     Size_t getDataSize() const { return m_dataSize; }

//   private:
//     Size_t m_headerSize;
//     Size_t m_dataSize;
//     Datas_t m_userData;
// };
// static_assert( sizeof( Header ) == 40 );

///////////////////////////////////////////////////////////////

class SRC_API StreamBase
{
  public:
    //    static const std::string s_defaultIpv4;
    //    static const int s_defaultPort;
    enum class ClientType { NONE = 0, STREAMER, STREAM_VIEWER, VIEWER, ASKER, KILLER, COUNT };
    SRC_API friend std::ostream& operator<<( std::ostream& os, const ClientType& clientType );

    enum class ClientMessage {
        NONE = 0,

        STREAMER_CLIENT_START,
        STREAMER_CLIENT_CLOSED,
        STREAMER_CLIENT_NEW_ACQ,
        STREAMER_CLIENT_INIT_SENSOR,

        STREAM_VIEWER_CLIENT_CLOSED,
        STREAM_VIEWER_CLIENT,

        VIEWER_CLIENT_CLOSED,
        VIEWER_CLIENT_SET_PROPERTY,

        ASKER_CLIENT_CLOSED,
        ASKER_CLIENT_GET_LIST_STREAMS,
        ASKER_CLIENT_GET_ACQ,

        CLIENT_SERVER_DOWN,
        STREAMER_CLIENT_NEW_STREAM_VIEWER,
        STREAMER_CLIENT_INITED,
        STREAMER_CLIENT_STREAM_VIEWER_INITED,

        NEW_RETAIN_DATA,
        FULLY_RETAINED_DATA,
        STREAMER_CLIENT_DEL_STREAM_VIEWER,

        VIEWER_CLIENT_STREAM_ADDED,

        //        KILL,

        COUNT
    };
    SRC_API friend std::ostream& operator<<( std::ostream& os, const ClientMessage& msg );

    enum class ServerMessage {
        NONE = 0,

        FOUND,
        NOT_FOUND,
        OK,

        SERVER_CLOSED,

        STREAMER_CLOSED,
        STREAMER_INITED,

        STREAM_VIEWER_CLOSED,
        STREAM_VIEWER_NEW_ACQ,

        VIEWER_CLOSED,
        VIEWER_NEW_STREAMER,
        VIEWER_DEL_STREAMER,
        VIEWER_SET_PROPERTY,

        ASKER_CLOSED,
        ASKER_LIST_STREAMS,

        STREAM_VIEWER_INITED,

        RETAINED_DATA_START,
        RETAINED_DATA_END,
        RETAINED_SHARED_TO_VIEWER,

        COUNT
    };
    SRC_API friend std::ostream& operator<<( std::ostream& os, const ServerMessage& msg );

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

    //    virtual const std::string & getDefaultIpv4() = 0;
    //    virtual int getDefaultPort() = 0;

    // const Header& getHeader() const;

  protected:
    StreamBase( const std::string& name, const std::string& ipv4, int port );
    // StreamBase( const char* name, const char* ipv4, int port, const Header& header );

    //    static std::string to_string( const ClientMessage& message );
    //    static std::string to_string( const ServerMessage& message );
    //    static const std::string getDefaultIpv4();
    //    static constexpr int getDefaultPort();

    // Header m_header;
    const std::string m_name;
    const std::string m_ipv4;
    const int m_port;
};

} // namespace io
} // namespace hub
