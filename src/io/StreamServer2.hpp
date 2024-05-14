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

#include <string>

#include "core/Configuration.hpp"
#include "InputOutputSocket.hpp"
#include "StreamBase.hpp"

namespace hub {
namespace io {

///
/// \brief The StreamServer2 class
///
class SRC_API StreamServer2 : public hub::io::StreamBase
{
  public:
    ///
    /// \brief stopServer
    /// \param ipv4
    /// \param port
    ///
    static void stopServer( std::string ipv4 = HUB_SERVICE_IPV4, int port = HUB_SERVICE_PORT ) {
        auto clientSock = hub::io::InputOutputSocket( hub::net::ClientSocket( ipv4, port ) );
        clientSock.write( hub::io::StreamBase::ClientType::KILLER );
    }

  protected:
    ///
    /// \brief StreamServer2
    /// \param name
    /// \param ipv4
    /// \param port
    ///
    StreamServer2( const std::string& name, const std::string& ipv4, int port );
};

} // namespace io
} // namespace hub
