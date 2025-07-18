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


#ifdef HUB_USE_MQTT

#include <mqtt/client.h>

#include "AskerInterface.hpp"
#include "io/StreamMqtt.hpp"

namespace hub {
namespace client {

///
/// \brief The AskerMqtt class
///
class AskerMqtt : public AskerInterface
{
  public:
    ///
    /// \brief AskerMqtt
    /// \param ipv4
    /// \param port
    ///
    explicit AskerMqtt( const std::string& ipv4 = io::StreamMqtt::s_defaultIpv4,
               int port                = io::StreamMqtt::s_defaultPort );

    ~AskerMqtt();

    std::list<std::pair<std::string, sensor::SensorSpec>> listStreams() override;
    sensor::Acquisition getAcquisition( const std::string& streamName ) override;

  private:
    template <class T>
    void read( T& t ) {
        memcpy( reinterpret_cast<unsigned char*>( &t ),
                m_inputMsgPtr->get_payload().data(),
                sizeof( T ) );
    }

    std::string m_hostName;
    std::unique_ptr<mqtt::client> m_client;
    mqtt::const_message_ptr m_inputMsgPtr;
};

} // namespace client
} // namespace hub

#endif
