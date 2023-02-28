#pragma once

// #include <functional>
// #include <regex>
#include <string>
// #include <thread>

// #include "Acquisition.hpp"
#include "Net/ClientSocket.hpp"
// #include "SensorSpec.hpp"
#include "Macros.hpp"

namespace hub {

///
/// \brief The Asker class
/// allow user to get some information from connected sensor to the server.
///
class SRC_API Asker
{
  public:
    ///
    /// \brief Asker
    /// \param ipv4 of the server you want to get information
    /// \param port of the server you want to get information
    ///
    explicit Asker( const std::string& ipv4 = net::s_defaultServiceIp,
           int port                = net::s_defaultServicePort );

    ~Asker();

    ///
    /// \brief listStreams
    /// \return
    /// list of connected sensors to the server
    ///
    std::list<std::pair<std::string, hub::SensorSpec>> listStreams() const;

//    ///
//    /// \brief getSensorSpec
//    /// \param streamName
//    /// \return
//    ///
//    hub::SensorSpec getSensorSpec(const std::string & streamName) const;

    ///
    /// \brief getAcquisition
    /// allow to get last acquired data from connected sensor to the server
    /// \param streamName
    /// \return
    ///
    hub::Acquisition getAcquisition(const std::string & streamName) const ;


    ///////////////////////////////////////////////////////////////////////////////////////////////

  private:
    net::ClientSocket m_sock;
};

} // namespace hub
