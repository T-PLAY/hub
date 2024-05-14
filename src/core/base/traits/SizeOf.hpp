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
*   - Gauthier Bouyjou (IRIT, Toulouse Tech Transfer)
*******************************************************************************/

#pragma once


#include <type_traits>
#include <string>

#include "core/base/Macros.hpp"
#include "core/base/Defines.hpp"
#include "TypeId.hpp"

namespace hub {

///
/// \brief Sizeof_t
///
using Sizeof_t = decltype(sizeof(int));

#if CPP_VERSION >= 20

template <class T>
constexpr auto sizeOf() {
    if constexpr ( requires { T::Size; } ) { return T::Size; }
    else {
        return sizeof( T );
    }
}

template <class... Ts>
requires( sizeof...( Ts ) > 1 ) constexpr auto sizeOf() {
    return ( sizeOf<Ts>() + ... );
}

#else

///
/// \brief has_Size_t
///
template <typename T>
using has_Size_t = decltype( T::Size );

template <typename T, typename = std::void_t<>>
struct has_Size : std::false_type {};

template <typename T>
struct has_Size<T, std::void_t<has_Size_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool has_Size_v = has_Size<T>::value;

////////////////////////

///
/// \brief has_size_t
///
template <typename T>
using has_size_t = decltype( std::declval<T>().size() );

template <typename T, typename = std::void_t<>>
struct has_size : std::false_type {};

template <typename T>
struct has_size<T, std::void_t<has_size_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool has_size_v = has_size<T>::value;


///
/// \brief sizeOf
///
template <class T>
REQUIRES( static constexpr, has_Size_v<T>, Sizeof_t )
sizeOf() {
    return T::Size;
}

///
/// \brief sizeOf
///
template <class T>
REQUIRES( static constexpr, !has_Size_v<T>, Sizeof_t )
sizeOf() {
    return sizeof( T );
}

///
/// \brief sizeOf
///
template <class... Ts>
REQUIRES( constexpr, sizeof...( Ts ) > 1, Sizeof_t )
sizeOf() {
    static_assert( sizeof...( Ts ) > 1 );
    return ( sizeOf<Ts>() + ... );
}

///
/// \brief sizeOf
/// \param t
///
template <class T>
REQUIRES( static constexpr, has_size_v<T>, Sizeof_t )
sizeOf( const T& t ) {
    return t.size();
}

///
/// \brief sizeOf
/// \param t
/// \param ts
///
template <class T, class... Ts>
REQUIRES( constexpr, sizeof...( Ts ) > 0, Sizeof_t )
sizeOf( const T& t, const Ts&... ts ) {

    if constexpr ( sizeof...( Ts ) > 0 ) { return sizeOf( t ) + sizeOf( ts... ); }
    else {
        return sizeOf( t );
    }
}

#endif

/////////////////////////////////////////////// PRETTY BYTES /////////////////////////////////////////////////

///
/// \brief pretty_bytes
/// \param bytes
/// \return
///
std::string pretty_bytes( hub::Size_t bytes );
#define PRETTY_BYTES( t ) hub::pretty_bytes( t )

}
