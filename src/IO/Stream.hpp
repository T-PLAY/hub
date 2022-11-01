#pragma once

#include <mutex>

#include "Interface.hpp"
#include "Net/ClientSocket.hpp"

namespace hub {
namespace io {

///
/// \brief The OutputStream class
/// communicates with the server application, and create new streamer with the according stream
/// name. A viewer connected with the server can show this new stream, and have the possibility to
/// open an InputStream with the same stream name.
///
class SRC_API OutputStream : public OutputInterface, public net::ClientSocket
{
  public:
    ///
    /// \brief OutputStream
    /// \param streamName
    /// \param clientSocket
    ///
    OutputStream( const std::string& streamName,
                  net::ClientSocket&& clientSocket = net::ClientSocket() );

#ifdef WIN32 // msvc warning C4250
  protected:
    void write( const unsigned char* data, size_t len ) const override;
    void read( unsigned char* data, size_t len ) const override;
    void close() override;
#endif
};

///
/// \brief The InputStream class
/// Allows an user to retrieve the data from an already created OutputStream.
/// An atream can be synchronize with an another stream.
///
class SRC_API InputStream : public InputInterface, public net::ClientSocket
{
  public:
    ///
    /// \brief InputStream
    /// \param streamName
    /// \param syncStreamName
    /// \param clientSocket
    ///
    InputStream( const std::string& streamName,
                 const std::string& syncStreamName = "",
                 net::ClientSocket&& clientSocket  = net::ClientSocket() );

#ifdef WIN32 // msvc warning C4250
  protected:
    void write( const unsigned char* data, size_t len ) const override;
    void read( unsigned char* data, size_t len ) const override;
    void close() override;
#endif

  protected:
    Acquisition getAcquisition( int acquisitionSize ) const override;
};

} // namespace io
} // namespace hub
