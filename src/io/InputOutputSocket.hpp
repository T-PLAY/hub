/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright © 2021-2024 Hub. All Rights Reserved.
 * @author Gauthier Bouyjou <gauthierbouyjou@aol.com>
 * @date 2023/11/20
 */

#pragma once

#include <string>

#include "core/io/InputOutput.hpp"
#include "net/ClientSocket.hpp"

namespace hub {
namespace io {

///
/// \brief The InputOutputSocket class
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

    ///
    /// \brief read
    /// \param data
    /// \param size
    ///
    void read( hub::Data_t* data, hub::Size_t size ) override {
        net::ClientSocket::read( data, size );
    }

    ///
    /// \brief write
    /// \param data
    /// \param size
    ///
    void write( const hub::Data_t* data, hub::Size_t size ) override {
        net::ClientSocket::write( data, size );
    }

    ///
    /// \brief close
    ///
    void close() override { disconnect(); };

    ///
    /// \brief isOpen
    /// \return
    ///
    bool isOpen() const override { return isConnected(); }

    ///
    /// \brief isEnd
    /// \return
    ///
    bool isEnd() const override { return false; }

    ///
    /// \brief clear
    ///
    void clear() override {}

    ///
    /// \brief setRetain
    /// \param retain
    ///
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
