#pragma once

#include <mutex>

#include "Interface.hpp"
#include "Net/ClientSocket.hpp"

namespace hub {
namespace io {

///
/// \brief The OutputStream class
/// communicates with the server application, and create new streamer with the according stream name.
/// A viewer connected with the server can show this new stream, and have the possibility to open an InputStream with
/// the same stream name.
///
class OutputStream : public OutputInterface, public net::ClientSocket
{
  public:
    ///
    /// \brief OutputStream
    /// \param streamName
    /// \param clientSocket
    ///
    OutputStream( const std::string& streamName,
                  net::ClientSocket&& clientSocket = net::ClientSocket() );
};

///
/// \brief The InputStream class
/// Allows an user to retrieve the data from an already created OutputStream.
/// An atream can be synchronize with an another stream.
///
class InputStream : public InputInterface, public net::ClientSocket
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

protected:
    Acquisition getAcquisition(int acquisitionSize) const override;
};

} // namespace io
} // namespace hub
