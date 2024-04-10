/*******************************************************************************
* Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
* Please visit https://www.irit.fr/tplay/.
*
* All rights reserved.
* This code belongs to tplay/hub project (https://github.com/T-PLAY/hub).
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*
* Initial Contributors:
*   - Gauthier Bouyjou
*******************************************************************************/

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
    explicit AskerServer2( const std::string& ipv4 = HUB_SERVICE_IPV4, int port = HUB_SERVICE_PORT );

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
