/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/30

#pragma once

#include "AskerInterface.hpp"
#include "io/InputOutputSocket.hpp"
#include "io/StreamServer2.hpp"

namespace hub {
namespace client {

///
/// \brief The AskerServer2 class
/// is an implement of AskerInterface
///
class SRC_API AskerServer2 : public hub::client::AskerInterface
{
  public:
    ///
    /// \brief AskerServer2
    /// \param ipv4
    /// \param port
    ///
    AskerServer2( const std::string& ipv4 = HUB_SERVICE_IPV4, int port = HUB_SERVICE_PORT );

    ~AskerServer2();

    ///
    /// \brief listStreams
    /// \return
    ///
    std::list<std::pair<std::string, sensor::SensorSpec>> listStreams() override;

    ///
    /// \brief getAcquisition
    /// \param streamName
    /// \return
    ///
    sensor::Acquisition getAcquisition( const std::string& streamName ) override;

  private:
    hub::io::InputOutputSocket m_sock;
};

} // namespace client
} // namespace hub
