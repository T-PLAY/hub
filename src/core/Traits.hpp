#pragma once

#include "Macros.hpp"

// #include <iostream>
#include <map>
#include <string>
#include <type_traits>
// #include <vector>

template <class Container, class T = std::decay_t<decltype( *std::declval<Container>().begin() )>>
auto toString( const Container& container ) {
    std::string str;
    str += "<" + TYPE_NAME( T ) + ">";
    str += "[";
    constexpr bool stringable   = requires( T t ) { str += t; } && !std::is_arithmetic_v<T>;
    constexpr bool toStringable = requires( T t ) { t.toString(); };

    constexpr auto nFirstData = 20;
    constexpr auto nLastData  = 8;
//    const auto iMax           = std::min( (int)container.size(), nFirstData );
    for ( size_t i = 0; i < container.size(); ++i ) {
        if ( nFirstData < i && i < container.size() - nLastData ) {
            if ( i == container.size() - nLastData - 1 ) { str += "... "; }
            continue;
        }
        const auto& el = container[i];
        if constexpr ( stringable ) { str += el; }
        else if constexpr ( toStringable ) { str += el.toString(); }
        else { str += std::to_string( el ); }
        //        if constexpr ( std::is_same_v<T, char> ) { str += container[i]; }
        //        else { str += std::to_string( container[i] ); }
        if ( i != container.size() - 1 ) str += " ";
        //    }
        //    if ( container.size() > nFirstData ) {
        //        const auto& lastEl = container.back();
        //        str += " ... ";
        //        if constexpr ( stringable ) { str += lastEl; }
        //        else if constexpr ( toStringable ) { str += lastEl.toString(); }
        //        else { str += std::to_string( lastEl ); }
    }
    str += "](" + PRETTY_BYTES( container.size() * sizeof( T ) ) + ")";
    //    str += "](" + std::to_string( container.size() ) + ")";
    return str;
}

template <class T, class U>
auto toString( const std::map<T, U>& container ) {
    std::string str;
    //    str += "<" + TYPE_NAME( T ) + ">";
    //    str += "std::map<" + TYPE_NAME( T ) + ", " + TYPE_NAME( U ) + ">";
    str += "[";
    constexpr bool stringableT   = requires( T t ) { str += t; } && !std::is_arithmetic_v<T>;
    constexpr bool toStringableT = requires( T t ) { t.toString(); };
    constexpr bool stringableU   = requires( U u ) { str += u; } && !std::is_arithmetic_v<U>;
    constexpr bool toStringableU = requires( U u ) { u.toString(); };

    //    constexpr auto nMaxDataToShow = 40;
    //    const auto iMax               = std::mincontainer.size(), nMaxDataToShow );
    //    for ( auto i = 0; i < iMax; ++i ) {
    size_t i = 0;
    for ( const auto& [key, value] : container ) {
        //        const auto& [key, value] = pair;

        str += "{";

        if constexpr ( stringableT ) { str += key; }
        else if constexpr ( toStringableT ) { str += key.toString(); }
        else { str += std::to_string( key ); }

        str += ": ";

        if constexpr ( stringableU ) { str += value; }
        else if constexpr ( toStringableU ) { str += value.toString(); }
        else { str += std::to_string( value ); }

        str += "}";

        if ( i != container.size() - 1 ) str += ", ";
        ++i;
    }
    str += "]";
    return str;
}

template <class T, class U>
auto toString( const std::pair<T, U>& pair ) {
    std::string str;
    //    str += "std::pair<" + TYPE_NAME( T ) + ", " + TYPE_NAME( U ) + ">";
    //    str += "[";
    constexpr bool stringableT   = requires( T t ) { str += t; } && !std::is_arithmetic_v<T>;
    constexpr bool toStringableT = requires( T t ) { t.toString(); };
    constexpr bool stringableU   = requires( U u ) { str += u; } && !std::is_arithmetic_v<U>;
    constexpr bool toStringableU = requires( U u ) { u.toString(); };

    const auto& [key, value] = pair;

    str += "{";

    if constexpr ( stringableT ) { str += key; }
    else if constexpr ( toStringableT ) { str += key.toString(); }
    else { str += std::to_string( key ); }

    str += ": ";

    if constexpr ( stringableU ) { str += value; }
    else if constexpr ( toStringableU ) { str += value.toString(); }
    else { str += std::to_string( value ); }

    str += "}";

    //    str += "]";
    return str;
}

template <class T, class U>
std::ostream& operator<<( std::ostream& os, const std::pair<T, U>& pair ) {
    os << toString( pair );
    return os;
}

template <class Container, class T = std::decay_t<decltype( *std::declval<Container>().begin() )>>
    requires( !std::is_same_v<Container, std::string> )
std::ostream& operator<<( std::ostream& os, const Container& container ) {
    os << toString( container );
    return os;
}

template <class T>
concept Stringable = requires( std::ostream& os, const T& t ) { os << t.toString(); };

template <class T>
    requires Stringable<T>
std::ostream& operator<<( std::ostream& os, const T& t ) {
    os << t.toString();
    return os;
}

namespace std {

#if CPLUSPLUS_VERSION < 20

template <typename T>
struct remove_cvref : std::remove_cv<std::remove_reference_t<T>> {};

template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

#endif

//////////////////////////////////

#if CPLUSPLUS_VERSION < 17

template <class T, class U>
static constexpr auto is_same_v = is_same<T, U>::value;

template <class T>
static constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

template <class T>
static constexpr bool is_array_v = is_array<T>::value;

#endif

} // namespace std

// namespace hub {

// #if ( __cplusplus < 201703L )
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

// template <typename T>
// concept printable = requires( T t ) {
//     { std::cout << t } -> std::same_as<std::ostream&>;
// };

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

// template <class T>
// std::size_t sizeofAll( T& ts ) {
//     //        zpp::bits::in input( m_serialBuff );
//     //        input( ts... ).or_throw();
//     return sizeof( T );
// }

// template <class T, class... Ts>
// std::size_t sizeofAll( T& t, Ts&... ts ) {
//     //        assert( isOpen() );
//     //        assert( !isEnd() );
//     //        read( m_serialBuff.data(), sizeof(Ts)... );
//     //        zpp::bits::in input( m_serialBuff );
//     //        input( ts... ).or_throw();
//     //        input( ts... ).or_throw();
//     return sizeof( T ) + sizeofAll( ts... );
// }

// #if CPLUSPLUS_VERSION >= 20 // concept
// template <class T>
// concept Printable = requires( T a ) { std::cout << a; };
// #else
// template <class T>
// static constexpr auto Printable = true;
// #endif

//} // namespace hub
