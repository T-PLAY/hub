#pragma once


#include <string>

#include "Acquisition.hpp"
#include "Macros.hpp"
#include "SensorSpec.hpp"
//#include "net/ClientSocket.hpp"

namespace hub {
namespace client {

///
/// \brief The Asker class
/// allow user to get some information from connected sensor to the server.
///
class SRC_API AskerInterface
{
  public:
//  protected:
//    ///
//    /// \brief Asker
//    /// \param ipv4 of the server you want to get information
//    /// \param port of the server you want to get information
//    ///
//    explicit AskerInterface( const std::string& ipv4 = net::s_defaultServiceIp,
//                    int port                = net::s_defaultServicePort );

//    ~AskerInterface();

    ///
    /// \brief listStreams
    /// \return
    /// list of connected sensors to the server
    ///
    virtual std::list<std::pair<std::string, hub::SensorSpec>> listStreams() = 0;

    ///
    /// \brief getAcquisition
    /// allow to get last acquired data from connected sensor to the server
    /// \param streamName
    /// \return
    ///
    virtual hub::Acquisition getAcquisition( const std::string& streamName ) = 0;

    ///////////////////////////////////////////////////////////////////////////////////////////////

  private:
//    net::ClientSocket m_sock;
};

} // namespace client
} // namespace hub
