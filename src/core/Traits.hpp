#pragma once

#include "Macros.hpp"

#include <type_traits>

namespace std {

#if CPLUSPLUS_VERSION < 20
template <typename T>
struct remove_cvref :std::remove_cv<std::remove_reference_t<T>> {};

template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;
#endif

#if CPLUSPLUS_VERSION < 17
template <class T, class U>
static constexpr auto is_same_v = is_same<T, U>::value;

template< class T >
static constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

template< class T >
static constexpr bool is_array_v = is_array<T>::value;
#endif

}


namespace hub {

//#if ( __cplusplus < 201703L )
#if CPLUSPLUS_VERSION < 17
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

#if defined( OS_MACOS ) && \
    CPLUSPLUS_VERSION <= 14 // std::void_t not supported by AppleClang (c++14)
namespace std {

template <typename... Ts>
struct make_void {
    typedef void type;
};

template <typename... Ts>
using void_t = typename make_void<Ts...>::type;

} // namespace std
#endif

// template <class T>
// constexpr bool isSimpleType() noexcept {
//     return std::is_same<int, T> {};
// }

// template <typename T>
////    using readable_t = decltype( std::declval<T>().read( std::declval<Input&>() ) );
// using serializable_t = decltype( std::declval<T&>().serialize() );

// template <typename T, typename = std::void_t<>>
// struct serializable : std::false_type {};

// template <typename T>
// struct serializable<T, std::void_t<serializable_t<T>>> : std::true_type {};

// template <typename T>
// static constexpr bool serializable_v = serializable<T>::value;

//template <typename T>
//concept printable = requires( T t ) {
//    { std::cout << t } -> std::same_as<std::ostream&>;
//};

// template <typename T>
// using isInput_t = decltype( T::isInput );
// using isInput_t = decltype( std::declval<T>().read( std::declval<hub::Data_t*>(),
// std::declval<hub::Size_t>() ) ); using isInput_t = decltype( std::declval<T>().read(
// std::declval<hub::Data_t*>(), std::declval<hub::Size_t>() ) );

// template <typename T, typename = std::void_t<>>
// struct isInput : std::false_type {};

// template <typename T>
// struct isInput<T, std::void_t<isInput<T>>> : std::true_type {};

// #include "core/Input.hpp"
// template <typename T>
////static constexpr bool isInput_v = isInput<T>::value;
// static constexpr bool isInput_v = std::is_base_of_v<hub::Input, T>;

// #include "core/Output.hpp"
// template <typename T>
////static constexpr bool isInput_v = isInput<T>::value;
// static constexpr bool isOutput_v = std::is_base_of_v<hub::Output, T>;

template <class T>
Size_t sizeofAll( T& ts ) {
    //        zpp::bits::in input( m_serialBuff );
    //        input( ts... ).or_throw();
    return sizeof( T );
}

template <class T, class... Ts>
Size_t sizeofAll( T& t, Ts&... ts ) {
    //        assert( isOpen() );
    //        assert( !isEnd() );
    //        read( m_serialBuff.data(), sizeof(Ts)... );
    //        zpp::bits::in input( m_serialBuff );
    //        input( ts... ).or_throw();
    //        input( ts... ).or_throw();
    return sizeof( T ) + sizeofAll( ts... );
}

} // namespace hub
