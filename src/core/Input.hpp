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
*   - Nicolas Mellado (IRIT)
*   - Gauthier Bouyjou (IRIT)
*******************************************************************************/

#pragma once

#include "Base.hpp"
#include "InputBase.hpp"
#include "Serializer.hpp"

namespace hub {

///
/// \brief The InputT class
/// is an implement of InputBase
/// able to use custom serializer as a template parameter
///
template <class SerializerT = Serializer>
class SRC_API InputT : public InputBase
{
  public:
    using InputBase::read;

    virtual ~InputT() = default;

  public:
    ///
    /// \brief read
    /// \tparam T	A packable object
    /// \return
    ///
    template <class T>
    typename std::enable_if_t<packable_v<T>, void> read( T& t ) {
        assert( isOpen() );
        assert( !isEnd() );
        if constexpr ( isPacket<T> ) { read( t.data(), t.size() ); }
        else {
            read( reinterpret_cast<Data_t*>( &t ), sizeof( T ) );
        }
#ifdef HUB_DEBUG_INPUT
        DEBUG_MSG( "\t" << HEADER << "read(packable: " << TYPE_NAME( t ) << ") = " << t );
#endif
    }

    ///
    /// \brief read
    /// \tparam T
    /// \return
    ///
    template <class T>
    typename std::enable_if_t<!packable_v<T> && readable_v<T>, void> read( T& t ) {
        assert( isOpen() );
        assert( !isEnd() );
        t.read( *this );

#ifdef HUB_DEBUG_INPUT
        DEBUG_MSG( "\t" << HEADER << "read(readable: " << TYPE_NAME( t ) << ") = " << t );
#endif
    }

    ///
    /// \brief read
    /// \tparam T
    /// \return
    ///
    template <class T>
    typename std::enable_if_t<!packable_v<T> && !readable_v<T>, void> read( T& t ) {
        assert( isOpen() );
        assert( !isEnd() );
        m_serializer.unpack( *this, t );

#ifdef HUB_DEBUG_INPUT
        DEBUG_MSG( "\t" << HEADER << "read(serial: " << TYPE_NAME( t ) << ") = " << t );
#endif
    }

    ///
    /// \brief readAll
    /// \tparam T
    /// \tparam Ts
    ///
    template <class T, class... Ts>
    void readAll( T& t, Ts&... ts ) {
        m_serializer.unpack( *this, t, ts... );
    }

  private:
    SerializerT m_serializer;
};

///
/// \brief Input
///
using Input = InputT<Serializer>;

} // namespace hub
