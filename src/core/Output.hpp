/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright © 2021-2024 Hub. All Rights Reserved.
 * @author Gauthier Bouyjou <gauthierbouyjou@aol.com>
 * @date 2023/10/14
 */

#pragma once

#include "Macros.hpp"
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
    /// \param t
    /// \return
    ///
    template <class T>
    typename std::enable_if<packable_v<T>>::type write( const T& t ) {
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
    /// \param t
    /// \return
    ///
    template <class T>
    typename std::enable_if<!packable_v<T> && writable_v<T>>::type write( const T& t ) {
#ifdef HUB_DEBUG_OUTPUT
        DEBUG_MSG( HEADER << "write(writable: " << TYPE_NAME( t ) << ") = " << t );
#endif
        assert( isOpen() );
        const_cast<T&>( t ).write( *this );
    }

    ///
    /// \brief write
    /// \param t
    /// \return
    ///
    template <class T>
    typename std::enable_if<!packable_v<T> && !writable_v<T>>::type write( const T& t ) {
#ifdef HUB_DEBUG_OUTPUT
        DEBUG_MSG( HEADER << "write(serial: " << TYPE_NAME( t ) << ") = " << t );
#endif
        assert( isOpen() );
        m_serializer.pack( *this, t );
    }

    ///
    /// \brief writeAll
    /// \param t
    /// \param ts
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
