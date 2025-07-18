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

#include <cassert>
#include <type_traits>

#include "Macros.hpp"

namespace std {

#if CPP_VERSION < 20

template <typename T>
struct remove_cvref : std::remove_cv<std::remove_reference_t<T>> {};

///
/// \brief remove_cvref_t
///
template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

#endif

//////////////////////////////////

#if CPP_VERSION < 17

template <class T, class U>
static constexpr auto is_same_v = is_same<T, U>::value;

template <class T>
static constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

template <class T>
static constexpr bool is_array_v = is_array<T>::value;

#endif

#if CPP_VERSION <= 14 // std::void_t not supported by AppleClang (c++14)

///
/// \brief string_view
///
using string_view = string;

template <class T>
static constexpr bool is_enum_v = is_enum<T>::value;

template <class T>
static constexpr bool is_pointer_v = is_pointer<T>::value;

template <class Base, class T>
static constexpr bool is_base_of_v = is_base_of<Base, T>::value;

#endif

#if defined( OS_MACOS ) && CPP_VERSION <= 14 // std::void_t not supported by AppleClang (c++14)

template <typename... Ts>
struct make_void {
    typedef void type;
};

template <typename... Ts>
using void_t = typename make_void<Ts...>::type;

#endif

} // namespace std

///////////////////////////////////////////////////////////////////////////////////

namespace hub {

#if CPP_VERSION >= 20

template <typename T>
concept isContainer = !std::is_same<T, std::string>() && requires( T t ) {
    t.begin();
    t.end();
    // std::begin( t );
    // std::end( t );
};

#else

///
/// \brief Object/Collection able to get begin iterator
///
template <typename T>
using has_begin_t = decltype( std::begin( std::declval<T>() ) );

/// \copydoc has_begin_t
template <typename T, typename = std::void_t<>>
struct has_begin : std::false_type {};

/// \copydoc has_begin_t
template <typename T>
struct has_begin<T, std::void_t<has_begin_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool has_begin_v = has_begin<T>::value;

////////////

///
/// \brief Object/Collection able to get end iterator
///
template <typename T>
using has_end_t = decltype( std::end( std::declval<T>() ) );

/// \copydoc has_end_t
template <typename T, typename = std::void_t<>>
struct has_end : std::false_type {};

/// \copydoc has_end_t
template <typename T>
struct has_end<T, std::void_t<has_end_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool has_end_v = has_end<T>::value;

///
/// \brief isContainer
///
template <typename T>
constexpr bool isContainer = !std::is_same<T, std::string>() && has_begin_v<T> && has_end_v<T>;

#endif // #if CPP_VERSION >= 20

static_assert( isContainer<std::vector<unsigned char>> );
static_assert( isContainer<std::vector<int>> );
static_assert( !isContainer<std::string> );
static_assert( !isContainer<const char*> );

} // namespace hub
