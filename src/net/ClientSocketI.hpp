/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright © 2021-2024 Hub. All Rights Reserved.
 * @author Gauthier Bouyjou <gauthierbouyjou@aol.com>
 * @date 2023/11/03
 */

#pragma once

#include "core/Macros.hpp"

namespace hub {
namespace net {

///
/// \brief The ClientSocketI class
///
class SRC_API ClientSocketI
{
  public:
    ///
    /// \brief setIpv4
    /// \param newIpv4
    ///
    virtual void setIpv4( const std::string& newIpv4 ) = 0;

    ///
    /// \brief getIpv4
    /// \return
    ///
    virtual const std::string& getIpv4() const         = 0;

    ///
    /// \brief setPort
    /// \param newPort
    ///
    virtual void setPort( int newPort ) = 0;

    ///
    /// \brief getPort
    /// \return
    ///
    virtual int getPort() const         = 0;

    ///
    /// \brief connect
    ///
    virtual void connect()           = 0;

    ///
    /// \brief isConnected
    /// \return
    ///
    virtual bool isConnected() const = 0;

    ///
    /// \brief disconnect
    ///
    virtual void disconnect()        = 0;

    ///
    /// \brief write
    /// \param data
    /// \param len
    ///
    virtual void write( const unsigned char* data, size_t len ) = 0;

    ///
    /// \brief read
    /// \param data
    /// \param len
    ///
    virtual void read( unsigned char* data, size_t len )        = 0;
};

} // namespace net
} // namespace hub
