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
#include <map>

#include "core/base/Macros.hpp"

#include "Array.hpp"
#include "Map.hpp"
#include "Span.hpp"
#include "Tuple.hpp"
#include "Vector.hpp"

////////////////////////////////////////////////////////////////////////

namespace hub {

//////////////////////////////////////// STRING /////////////////////////////////////////////

#if CPP_VERSION >= 20

template <class T>
concept StringAddable = requires( std::string str, const T& t ) {
    str += t;
};

#else

///
/// \brief is_string_t
///
template <typename T>
using is_string_t = decltype( std::declval<std::string>() += std::declval<T>() );

template <typename T, typename = std::void_t<>>
struct is_string : std::false_type {};

template <typename T>
struct is_string<T, std::void_t<is_string_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool is_string_v = is_string<T>::value;

template <class T>
static constexpr auto StringAddable = is_string_v<T> && !std::is_arithmetic_v<T>;

#endif

static_assert( StringAddable<std::string> );
#if CPP_VERSION < 20
static_assert( !StringAddable<int> );
static_assert( !StringAddable<char> );
#endif
static_assert( StringAddable<char*> );
static_assert( !StringAddable<std::vector<int>> );
static_assert( !StringAddable<std::vector<char>> );
static_assert( !StringAddable<std::vector<unsigned char>> );

/////////////////////////////////

#if CPP_VERSION >= 20
template <class T>
concept hasToString = requires( std::ostream& os, const T& t ) {
    os << t.toString();
};

#else

///
/// \brief has_toString_t
///
template <typename T>
using has_toString_t = decltype( std::declval<T>().toString() );

template <typename T, typename = std::void_t<>>
struct has_toString : std::false_type {};

template <typename T>
struct has_toString<T, std::void_t<has_toString_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool has_toString_v = has_toString<T>::value;

template <class T>
static constexpr auto hasToString = has_toString_v<T>;

#endif

    ///////////////////////////////

#if CPP_VERSION >= 20
template <class T>
concept StdToStringable = requires( std::ostream& os, const T& t ) {
    std::to_string( t );
};

template <class T>
concept toStringable_v = requires( std::ostream& os, const T& t ) {
    toString( t );
};

#else

///
/// \brief stdToStringable_t
///
template <typename T>
using stdToStringable_t = decltype( std::to_string( std::declval<T>() ) );

template <typename T, typename = std::void_t<>>
struct stdToStringable : std::false_type {};

template <typename T>
struct stdToStringable<T, std::void_t<stdToStringable_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool stdToStringable_v = stdToStringable<T>::value;

template <class T>
static constexpr auto StdToStringable = stdToStringable_v<T>;

////////////////////////////////

///
/// \brief toStringable_t
///
template <typename T>
using toStringable_t = decltype( toString( std::declval<T>() ) );

template <typename T, typename = std::void_t<>>
struct toStringable : std::false_type {};

template <typename T>
struct toStringable<T, std::void_t<toStringable_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool toStringable_v = toStringable<T>::value;

#endif

static_assert( !StdToStringable<std::string> );
static_assert( StdToStringable<int> );
static_assert( StdToStringable<char> );
static_assert( StdToStringable<bool> );
static_assert( !StdToStringable<char*> );
static_assert( !StdToStringable<std::vector<int>> );
static_assert( !StdToStringable<std::vector<unsigned char>> );

static auto to_string( const std::string& str ) {
    return "\"" + str + "\"";
}
static auto to_string( const char* str ) -> std::string {
    return std::string( "\"" ) + str + "\"";
}
static auto to_string( unsigned char c ) -> std::string {
    return std::to_string( c );
}
static auto to_string( bool b ) -> std::string {
    return ( b ) ? ( "true" ) : ( "false" );
}

template <class T>
static typename std::enable_if_t<StringAddable<T>, std::string> to_string( const T& t ) {
    static_assert( !std::is_same_v<T, std::string> );
    static_assert( !std::is_same_v<T, const char*> );
    static_assert( !std::is_same_v<T, char*> );
    std::string str;
    str += t;
    return str;
}

///
/// \brief to_string
/// \param t
/// \return
///
template <class T>
typename std::enable_if_t<hasToString<T>, std::string> to_string( const T& t ) {
    static_assert( !std::is_same_v<T, std::string> );
    static_assert( !std::is_same_v<T, const char*> );
    static_assert( !std::is_same_v<T, char*> );
    return t.toString();
}

///
/// \brief to_string
/// \param t
/// \return
///
template <class T>
typename std::enable_if_t<toStringable_v<T>, std::string> to_string( const T& t ) {
    static_assert( !std::is_same_v<T, std::string> );
    static_assert( !std::is_same_v<T, const char*> );
    static_assert( !std::is_same_v<T, char*> );
    return toString( t );
}

///
/// \brief to_string
/// \param t
/// \return
///
template <class T>
static typename std::enable_if_t<StdToStringable<T>, std::string> to_string( const T& t ) {
    static_assert( !std::is_same_v<T, std::string> );
    static_assert( !std::is_same_v<T, const char*> );
    static_assert( !std::is_same_v<T, char*> );
    return std::to_string( t );
}

///
/// \brief to_string
/// \param container
/// \return
///
template <class Container, class T = std::decay_t<decltype( *std::declval<Container>().begin() )>>
static typename std::enable_if_t<isContainer<Container>, std::string>
to_string( const Container& container ) {
    std::string str;
    str += "<" + TYPE_NAME( T() ) + ">";
    str += "[";

    constexpr auto nFirstData = 30;
    constexpr auto nLastData  = 8;
    auto it                   = container.begin();
    size_t i                  = 0;
    while ( it != container.end() ) {
        if ( nFirstData < i && i < container.size() - nLastData ) {
            if ( i == container.size() - nLastData - 1 ) { str += "... "; }
            ++it;
            ++i;
            continue;
        }
        const auto& el = *it;

        str += to_string( el );

        if ( i != container.size() - 1 ) str += " ";
        ++it;
        ++i;
    }
    str += "](" + PRETTY_BYTES( container.size() * sizeof( T ) ) + ")";
    return str;
}

template <class T, class U>
static auto to_string( const std::pair<T, U>& pair ) {
    const auto& [key, value] = pair;
    return "{" + to_string( key ) + ": " + to_string( value ) + "}";
}

template <class T, class U>
static auto to_string( const std::map<T, U>& map ) {
    std::string str;
    str += "[";
    size_t i = 0;
    for ( const auto& pair : map ) {
        str += to_string( pair );
        if ( i != map.size() - 1 ) str += ", ";
        ++i;
    }
    str += "]";
    return str;
}

template <class T>
static auto to_string( const std::set<T>& set ) {
    std::string str;
    str += "(";
    size_t i = 0;
    for ( const auto& el : set ) {
        str += to_string( el );
        if ( i != set.size() - 1 ) str += "; ";
        ++i;
    }
    str += ")";
    return str;
}

template <Size_t tupleSize, Size_t idx, class... Ts>
static void to_string_recurse( const std::tuple<Ts...>& tuple, std::string& str ) {
    str += ", " + to_string( std::get<idx>( tuple ) );
    if constexpr ( idx + 1 < tupleSize ) {
        to_string_recurse<tupleSize, idx + 1, Ts...>( tuple, str );
    }
}

template <class... Ts>
static std::string to_string( const std::tuple<Ts...>& tuple ) {
    static_assert( sizeof...( Ts ) > 0 );

    std::string str = "(";
    str += to_string( std::get<0>( tuple ) );
    if constexpr ( sizeof...( Ts ) > 1 ) {
        to_string_recurse<sizeof...( Ts ), 1, Ts...>( tuple, str );
    }
    str += ")";
    return str;
}

template <char delimiter, class T, class... Ts>
static void to_string_reduce( std::string& str, const T& t, const Ts&... ts ) {
    str += to_string( t );
    if constexpr ( sizeof...( Ts ) > 0 ) {
        if constexpr ( delimiter == ' ' ) { str += delimiter; }
        else {
            str += std::string( " " ) + delimiter + " ";
        }
        to_string_reduce<delimiter>( str, ts... );
    }
}

template <char delimiter = ' ', class... Ts>
static std::string to_string( const Ts&... ts ) {
    std::string str;
    to_string_reduce<delimiter>( str, ts... );
    return str;
}

///
/// \brief hubToStringable_t
///
template <typename T>
using hubToStringable_t = decltype( hub::to_string( std::declval<T>() ) );

template <typename T, typename = std::void_t<>>
struct hubToStringable : std::false_type {};

template <typename T>
struct hubToStringable<T, std::void_t<hubToStringable_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool hubToStringable_v = hubToStringable<T>::value;

#if CPP_VERSION < 17
template <class...>
struct is_one_of : std::false_type {};
template <class T1, class T2>
struct is_one_of<T1, T2> : std::is_same<T1, T2> {};
template <class T1, class T2, class... Ts>
struct is_one_of<T1, T2, Ts...> : std::conditional<std::is_same<T1, T2>::value,
                                                   std::is_same<T1, T2>,
                                                   is_one_of<T1, Ts...>>::type {};
#else
template <class T1, class... Ts>
constexpr bool is_one_of() noexcept {
    return ( std::is_same<T1, Ts> {} || ... );
}
#endif


} // namespace hub

template <class T>
typename std::enable_if_t<hub::hasToString<T>, std::ostream&> operator<<( std::ostream& os,
                                                                          const T& t ) {
    os << t.toString();
    return os;
}

template <class T>
typename std::enable_if_t<hub::toStringable_v<T>, std::ostream&> operator<<( std::ostream& os,
                                                                             const T& t ) {
    os << toString( t );
    return os;
}

template <class T>
std::ostream& operator<<( std::ostream& os, const std::vector<T>& vector ) {
    os << hub::to_string( vector );
    return os;
}

template <class T>
std::ostream& operator<<( std::ostream& os, const std::set<T>& set ) {
    os << hub::to_string( set );
    return os;
}

template <class T, std::size_t size>
std::ostream& operator<<( std::ostream& os, const std::array<T, size>& array ) {
    os << hub::to_string( array );
    return os;
}

template <class First, class Second>
std::ostream& operator<<( std::ostream& os, const std::pair<First, Second>& pair ) {
    os << hub::to_string( pair );
    return os;
}

template <class Key, class Value>
std::ostream& operator<<( std::ostream& os, const std::map<Key, Value>& map ) {
    os << hub::to_string( map );
    return os;
}

template <class... Ts>
std::ostream& operator<<( std::ostream& os, const std::tuple<Ts...>& tuple ) {
    os << hub::to_string( tuple );
    return os;
}
