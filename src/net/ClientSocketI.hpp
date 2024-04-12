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

#include "core/Base.hpp"

namespace hub {
namespace net {

///
/// \brief The ClientSocketI class
/// is an interface describing client socket basic features.
/// Please use this interface if you want to propose more efficient socket implement than the current.
/// We can compare during test and integrate your code if it more efficient.
///
class SRC_API ClientSocketI
{
  public:
    ///
    /// \brief Setter function
    /// \param newIpv4 [in] to set
    ///
    virtual void setIpv4( const std::string& newIpv4 ) = 0;

    ///
    /// \brief Getter function
    /// \return ip of server
    ///
    virtual const std::string& getIpv4() const         = 0;

    ///
    /// \brief Setter function
    /// \param newPort [in] to set
    ///
    virtual void setPort( int newPort ) = 0;

    ///
    /// \brief Getter function
    /// \return port of server
    ///
    virtual int getPort() const         = 0;

    ///
    /// \brief Try connecting to the server
    ///
    virtual void connect()           = 0;

    ///
    /// \brief Check if connected
    /// \return if connected to the server
    ///
    virtual bool isConnected() const = 0;

    ///
    /// \brief Disconnect to the server
    ///
    virtual void disconnect()        = 0;

    ///
    /// \brief write packet to the server
    /// \param data [in] pointer of data array
    /// \param len [in] size of data array
    ///
    virtual void write( const unsigned char* data, size_t len ) = 0;

    ///
    /// \brief read packet from the server
    /// \param data [out] pointer of data array
    /// \param len [in] size of data array
    ///
    virtual void read( unsigned char* data, size_t len )        = 0;
};

} // namespace net
} // namespace hub
