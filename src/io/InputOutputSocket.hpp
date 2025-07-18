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


#include "core/io/InputOutput.hpp"
#include "net/ClientSocket.hpp"

namespace hub {
namespace io {

///
/// \brief The InputOutputSocket class
/// is a bidirectional connection using network socket
///
class InputOutputSocket : public InputOutput<>, public net::ClientSocket
{
  public:
    using InputOutput<>::read;
    using InputOutput<>::write;

    ///
    /// \brief not_endable
    ///
    static struct {
    } not_endable;

    ///
    /// \brief InputOutputSocket
    /// \param clientSocket
    ///
    explicit InputOutputSocket( net::ClientSocket&& clientSocket ) :
        net::ClientSocket( std::move( clientSocket ) ) {}

    ///
    /// \brief InputOutputSocket
    ///
    InputOutputSocket( InputOutputSocket&& )      = default;

    InputOutputSocket( const InputOutputSocket& ) = delete;

    void read( hub::Data_t* data, hub::Size_t size ) override {
        net::ClientSocket::read( data, size );
    }

    void write( const hub::Data_t* data, hub::Size_t size ) override {
        net::ClientSocket::write( data, size );
    }

    void close() override { disconnect(); };

    bool isOpen() const override { return isConnected(); }

    bool isEnd() const override { return false; }

    void clear() override {}

    void setRetain( bool retain ) override {}

    ///
    /// \brief operator ==
    /// \param inputOutputSocket
    /// \return
    ///
    bool operator==( const InputOutputSocket& inputOutputSocket ) const {
        return m_fdSock == inputOutputSocket.m_fdSock;
    }
};

} // namespace io
} // namespace hub
