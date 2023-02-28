#pragma once

#include <mutex>

#include "Interface.hpp"
#include "Net/ClientSocket.hpp"

namespace hub {
namespace io {

///
/// \brief The OutputStream class
/// Describes an output communication to the server.
///
class SRC_API OutputStream : public OutputInterface, public net::ClientSocket
{
  public:
    ///
    /// \brief OutputStream
    /// is used to instantiate an OutputSensor.
    /// \param streamName
    /// [in] is an unique identifier name of stream
    /// \param clientSocket
    /// [in] is an existing connection to a hub server
    /// \warning The hub service must be accessible, that means the
    /// server hub is running on a machine you know the ip and the oppened port of the service.
    /// \warning Stream you want to start linking (by it name) needs to not exist in the server hub.
    /// Streaming with most of one sender is not allowed.
    /// \exception hub::net::Socket::exception
    /// when the server is not found or by loosing connection to the server.
    /// Also occur when stream you want to link is already started in the server.
    ///
    explicit OutputStream( const std::string& streamName,
                           net::ClientSocket&& clientSocket = net::ClientSocket() );

#ifdef WIN32 // msvc warning C4250
  protected:
    void write( const unsigned char* data, size_t len ) const override;
    void read( unsigned char* data, size_t len ) const override;
    void close() const override;
    bool isOpen() const override;
    bool isEnd() const override;

#endif
};

} // namespace io
} // namespace hub
