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
#include <thread>

#include "ViewerInterface.hpp"
#include "io/input/InputStreamServer2.hpp"

namespace hub {
namespace client {

///
/// \brief The ViewerServer2 class
/// is an implement of ViewerInterface compatible with Server implement
///
class ViewerServer2 : public hub::client::ViewerInterface<input::InputStreamServer2>
{
  public:
    ///
    /// \copydoc hub::client::ViewerInterface<input::InputStreamServer2>::ViewerInterface
    ///
    SRC_API explicit ViewerServer2( const std::string& name,
                                    ViewerHandler&& viewerHandler,
                                    const std::string& serverIpv4 = HUB_SERVICE_IPV4,
                                    int serverPort                = HUB_SERVICE_PORT,
                                    bool autoConnect        = true );

    SRC_API ~ViewerServer2();

    void setIpv4( const std::string& serverIpv4 ) override;

    void setPort( int serverPort ) override;

    void setProperty( const std::string& streamName,
                      const std::string& objectName,
                      int property,
                      const Any& value ) override;

    void setAutoConnect( bool autoConnect ) override;

  private:
    void threadRoutine();

    std::thread m_thread;
    bool m_threadRunning = false;
    bool m_stopThread    = false;

    hub::io::InputOutputSocket m_sock;
};

} // namespace client
} // namespace hub
