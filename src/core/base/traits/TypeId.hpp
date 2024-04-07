
#pragma once

#include <type_traits>
#include <typeinfo>

#include "core/base/Macros.hpp"

namespace hub {

///
/// \brief TypeId_t
///
using TypeId_t = int;

///
/// \brief has_id_t
///
template <typename T>
using has_id_t = decltype( T::id );

template <typename T, typename = std::void_t<>>
struct has_id : std::false_type {};

template <typename T>
struct has_id<T, std::void_t<has_id_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool has_id_v = has_id<T>::value;

namespace Types {
enum Cpp : TypeId_t { NONE = 0, INT /* 1 */, BOOL /* 2 */, Cpp_Count /* 3 */ };
}
static_assert( Types::Cpp_Count == 3 );

///
/// \brief getTypeId
/// \return
///
template <class T>
static constexpr typename std::enable_if_t<has_id_v<T>, TypeId_t> getTypeId( const T&  ) {
    return T::id;
}

///
/// \brief getTypeId
///
template <class T>
static constexpr typename std::enable_if_t<!has_id_v<T>, TypeId_t> getTypeId( const T& ) {
    return typeid( T ).hash_code();
}

///
/// \brief getTypeId
/// \return
///
constexpr auto getTypeId( int ) -> TypeId_t {
    return Types::Cpp::INT;
}

///
/// \brief getTypeId
/// \return
///
constexpr auto getTypeId( bool ) -> TypeId_t {
    return Types::Cpp::BOOL;
}

#define TYPE_ID( _Type_ ) hub::getTypeId( _Type_() )

#if CPP_VERSION >= 20
#    define REQUIRES( _CONST_, _COND_, _TYPE_ ) requires( _COND_ ) _CONST_ _TYPE_
#else
#    define REQUIRES( _CONST_, _COND_, _TYPE_ ) \
        _CONST_ typename std::enable_if_t<( _COND_ ), _TYPE_>
#endif


}
