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

#include "core/Base.hpp"
#include "OutputBase.hpp"
#include "Serializer.hpp"

// Todo getHeader() = 0
//#include "io/Header.hpp"

namespace hub {

///
/// \brief The OutputT class
/// is an implement of OutputBase
/// able to use custom serializer as a template parameter
///
template <class SerializerT = Serializer>
class SRC_API OutputT : public OutputBase
{
  public:
    using OutputBase::write;

    virtual ~OutputT() = default;

//    virtual const io::Header & getHeader() const = 0;

    ///
    /// \brief write
    /// \tparam t
    /// \return
    ///
    template <class T>
    typename std::enable_if_t<packable_v<T>, void> write( const T& t ) {
#ifdef HUB_DEBUG_OUTPUT
        DEBUG_MSG( HEADER << "write(packable: " << TYPE_NAME( t ) << ") = " << t );
#endif
        assert( isOpen() );
        if constexpr ( isPacket<T> ) { write( t.data(), t.size() ); }
        else {
            write( reinterpret_cast<const Data_t*>( &t ), sizeof( T ) );
        }
    }

    ///
    /// \brief write
    /// \tparam t
    /// \return
    ///
    template <class T>
    typename std::enable_if_t<!packable_v<T> && writable_v<T>, void> write( const T& t ) {
#ifdef HUB_DEBUG_OUTPUT
        DEBUG_MSG( HEADER << "write(writable: " << TYPE_NAME( t ) << ") = " << t );
#endif
        assert( isOpen() );
        const_cast<T&>( t ).write( *this );
    }

    ///
    /// \brief write
    /// \tparam t
    /// \return
    ///
    template <class T>
    typename std::enable_if_t<!packable_v<T> && !writable_v<T>, void> write( const T& t ) {
#ifdef HUB_DEBUG_OUTPUT
        DEBUG_MSG( HEADER << "write(serial: " << TYPE_NAME( t ) << ") = " << t );
#endif
        assert( isOpen() );
        m_serializer.pack( *this, t );
    }

    ///
    /// \brief writeAll
    /// \tparam t
    /// \tparam ts
    ///
    template <class T, class... Ts>
    void writeAll( const T& t, const Ts&... ts ) {
        m_serializer.pack( *this, t, ts... );
    }

  private:
    SerializerT m_serializer;
};

///
/// \brief Output
///
using Output = OutputT<>;

} // namespace hub
