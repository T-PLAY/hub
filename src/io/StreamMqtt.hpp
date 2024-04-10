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

#include "StreamBase.hpp"

namespace hub {
namespace io {

///
/// \brief The StreamMqtt class
///
class StreamMqtt : public StreamBase
{
  public:

    ///
    /// \brief s_defaultIpv4
    ///
    static const std::string s_defaultIpv4;

    ///
    /// \brief s_defaultPort
    ///
    static const int s_defaultPort;

    ///
    /// \brief s_topicStream
    ///
    static const std::string s_topicStream;

    ///
    /// \brief s_topicViewer
    ///
    static const std::string s_topicViewer;

    ///
    /// \brief s_topicEvents
    ///
    static const std::string s_topicEvents;

    ///
    /// \brief s_bufferAcqSize
    ///
    static constexpr int s_bufferAcqSize = 100;

  protected:
    ///
    /// \brief StreamMqtt
    /// \param name
    /// \param ipv4
    /// \param port
    ///
    StreamMqtt( const std::string& name, const std::string& ipv4, int port );

    ///
    /// \brief StreamMqtt
    /// \param name
    /// \param ipv4
    /// \param port
    ///
    StreamMqtt( const char* name, const char* ipv4, int port );

    ///
    /// \brief m_iAcq
    ///
    int m_iAcq = 0;
};

} // namespace io
} // namespace hub
