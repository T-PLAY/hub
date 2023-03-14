#pragma once

#include <mutex>

#include "Output.hpp"
#include "net/ClientSocket.hpp"

namespace hub {
namespace io {

///
/// \brief The OutputStream class
/// Describes an output communication to the server.
///
class SRC_API OutputStream : public Output
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

    //#ifdef WIN32 // msvc warning C4250
  protected:
    void write( const unsigned char* data, size_t len ) override;
    //    void read( unsigned char* data, size_t len ) const override;
    void close() override;
    bool isOpen() const override;
    //    bool isEnd() const override;

    //#endif

  private:
    net::ClientSocket m_clientSocket;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#ifdef WIN32
inline void OutputStream::write( const unsigned char* data, size_t len ) {
    //    net::ClientSocket::write( data, len );
    m_clientSocket.write( data, len );
}

// void OutputStream::read( unsigned char* data, size_t len ) const {
//    net::ClientSocket::read( data, len );
//    m_clientSocket.read(data, len);
//}

inline void OutputStream::close() {
    //    net::ClientSocket::close();
    m_clientSocket.close();
}

inline bool OutputStream::isOpen() const {
    //    return net::ClientSocket::isOpen();
    return m_clientSocket.isOpen();
}

// inline bool OutputStream::isEnd() const {
////    return net::ClientSocket::isEnd();
//    return m_clientSocket.isEnd();
//}

//#endif

} // namespace io
} // namespace hub
