/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright © 2021-2024 Hub. All Rights Reserved.
 * @author Gauthier Bouyjou <gauthierbouyjou@aol.com>
 * @date 2023/11/30
 */

#pragma once

#ifdef HUB_USE_MQTT

#    include "ViewerInterface.hpp"
#    include "io/input/InputStreamMqtt.hpp"

namespace hub {
namespace client {

class SRC_API ViewerMqtt : public ViewerInterface<input::InputStreamMqtt>
{
  public:
    ViewerMqtt( const std::string& name,
                ViewerHandler&& viewerHandler,
                const std::string& ipv4 = input::InputStreamMqtt::s_defaultIpv4,
                int port                = input::InputStreamMqtt::s_defaultPort );

    ~ViewerMqtt();

    ///////////////////////////////////////////////////////////////////////////////////////////////

  private:
    std::thread m_thread;
    bool m_stopThread = false;
    std::unique_ptr<mqtt::client> m_client;
    mqtt::const_message_ptr m_inputMsgPtr;
    mqtt::message_ptr m_outputMsgPtr;
};

} // namespace client
} // namespace hub

#endif
