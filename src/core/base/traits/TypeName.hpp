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
#include <vector>
#include <map>
#include <set>

#include "core/base/Macros.hpp"

#ifdef HUB_USE_BOOST
#    include <boost/type_index.hpp>
#endif

namespace hub {

///
/// \brief Object with name() feature
///
template <typename T>
using has_name_t = decltype( T::name() );

/// \copydoc has_name_t
template <typename T, typename = std::void_t<>>
struct has_name : std::false_type {};

/// \copydoc has_name_t
template <typename T>
struct has_name<T, std::void_t<has_name_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool has_name_v = has_name<T>::value;

//////////////

///
/// \brief Object which can be called by name function
///
template <typename T>
using nameable_t = decltype( name( std::declval<T>() ) );

/// \copydoc nameable_t
template <typename T, typename = std::void_t<>>
struct nameable : std::false_type {};

/// \copydoc nameable_t
template <typename T>
struct nameable<T, std::void_t<nameable_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool nameable_v = nameable<T>::value;

template <class T>
static typename std::enable_if_t<!nameable_v<T> && !has_name_v<T>, std::string>
typeName( const T&  ) {
#ifdef HUB_USE_BOOST
    return boost::typeindex::type_id<typeof( T )>().pretty_name();
#else
    return typeid( T ).name();
#endif
}

template <class T>
static typename std::enable_if_t<!nameable_v<T> && has_name_v<T>, std::string>
typeName( const T&  ) {
    return T::name();
}

template <class T>
static typename std::enable_if_t<nameable_v<T>, std::string> typeName( const T& t ) {
    static_assert( !has_name_v<T> );
    return name( t );
}

static std::string typeName( const std::string& ) {
    return "string";
}
static std::string typeName( int ) {
    return "int";
}
static std::string typeName( double ) {
    return "double";
}
static std::string typeName( bool ) {
    return "bool";
}
static std::string typeName( float ) {
    return "float";
}
static std::string typeName( char ) {
    return "char";
}
static std::string typeName( unsigned char ) {
    return "uchar";
}
static std::string typeName( const char* ) {
    return "cstr";
}
static std::string typeName( long long ) {
    return "long long";
}

template <class T>
static std::string typeName( const std::vector<T>& ) {
    return "vector<" + typeName( T() ) + ">";
}

template <class T>
static std::string typeName( const std::set<T>& ) {
    return "set<" + typeName( T() ) + ">";
}

template <class First, class Second>
static std::string typeName( const std::pair<First, Second>& ) {
    return "pair<" + typeName( First() ) + ", " + typeName( Second() ) + ">";
}

template <class Key, class Value>
static std::string typeName( const std::map<Key, Value>& ) {
    return "map<" + typeName( Key() ) + ", " + typeName( Value() ) + ">";
}

template <class T, class... Ts>
static void typeName_recurse( std::string& str ) {
    str += ", " + typeName( T() );
    if CONSTEXPR17 ( sizeof...( Ts ) > 0 ) { typeName_recurse<Ts...>( str ); }
}

template <class T, class... Ts>
static std::string typeName( const std::tuple<T, Ts...>& ) {
    std::string str = "tuple<";
    str += typeName( T() );
#if CPP_VERSION > 14
    if CONSTEXPR17 ( sizeof...( Ts ) > 0 ) { typeName_recurse<Ts...>( str ); }
#endif
    str += ">";
    return str;
}

template <class T, class... Ts>
static std::string typeName() {
    std::string str;
    str += typeName( T() );
    if constexpr ( sizeof...( Ts ) > 0 ) { str += ", " + typeName<Ts...>(); }
    return str;
}


#define TYPE_NAME( _Type_ ) hub::typeName( _Type_ )


}
