#pragma once

#include "Macros.hpp"

#include <type_traits>

namespace hub {

#if ( __cplusplus < 201703L )
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

#if defined(OS_MACOS) && CPLUSPLUS_VERSION <= 14 // std::void_t not supported by AppleClang (c++14)
namespace std {

template<typename... Ts>
struct make_void { typedef void type; };

template <typename ... Ts>
using void_t = typename make_void<Ts...>::type;

}
#endif

template <typename T>
using isPackable_t = decltype( T::packable );

template <typename T, typename = std::void_t<>>
struct isPackable : std::false_type {};

template <typename T>
struct isPackable<T, std::void_t<isPackable_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool isPackable_v = isPackable<T>::value;

template <typename T>
static constexpr bool isSimpleType_v = std::is_same<T, int>::value;
//template <class T>
//constexpr bool isSimpleType() noexcept {
//    return std::is_same<int, T> {};
//}

//template <typename T>
//using isInput_t = decltype( T::isInput );
//using isInput_t = decltype( std::declval<T>().read( std::declval<hub::Data_t*>(), std::declval<hub::Size_t>() ) );
//using isInput_t = decltype( std::declval<T>().read( std::declval<hub::Data_t*>(), std::declval<hub::Size_t>() ) );

//template <typename T, typename = std::void_t<>>
//struct isInput : std::false_type {};

//template <typename T>
//struct isInput<T, std::void_t<isInput<T>>> : std::true_type {};

//#include "core/Input.hpp"
//template <typename T>
////static constexpr bool isInput_v = isInput<T>::value;
//static constexpr bool isInput_v = std::is_base_of_v<hub::Input, T>;

//#include "core/Output.hpp"
//template <typename T>
////static constexpr bool isInput_v = isInput<T>::value;
//static constexpr bool isOutput_v = std::is_base_of_v<hub::Output, T>;

} // namespace hub
