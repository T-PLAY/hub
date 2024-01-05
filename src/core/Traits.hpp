#pragma once

#include "Macros.hpp"

#include "traits/Array.hpp"
#include "traits/Map.hpp"
#include "traits/Span.hpp"
#include "traits/Tuple.hpp"
#include "traits/Vector.hpp"
#include "traits/std_any.hpp"

// #include <iostream>
#include <map>
#include <string>
#include <type_traits>
// #include <vector>

////////////////////////////////////////////////////////////////////////

namespace hub {

// constexpr bool stringable   = requires( T t ) { str += t; } && !std::is_arithmetic_v<T>;
// constexpr bool hasToString = requires( T t ) { t.toString(); };

#if CPP_VERSION >= 20

template <class T>
concept StringAddable = requires( std::string str, const T& t ) { str += t; };

#else

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
static_assert( !StringAddable<int> );
static_assert( !StringAddable<char> );
static_assert( StringAddable<char*> );
static_assert( !StringAddable<std::vector<int>> );
static_assert( !StringAddable<std::vector<char>> );
static_assert( !StringAddable<std::vector<unsigned char>> );

/////////////////////////////////

#if CPP_VERSION >= 20
template <class T>
concept hasToString = requires( std::ostream& os, const T& t ) { os << t.toString(); };

#else

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
concept stdToStringable = requires( std::ostream& os, const T& t ) { os << t.toString(); };

#else

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
    return std::string( "'" ) + std::to_string( c ) + "'";
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

template <class T>
typename std::enable_if_t<hasToString<T>, std::string> to_string( const T& t ) {
    static_assert( !std::is_same_v<T, std::string> );
    static_assert( !std::is_same_v<T, const char*> );
    static_assert( !std::is_same_v<T, char*> );
    // static_assert( !StringAddable<T> );
    return t.toString();
}

template <class T>
static typename std::enable_if_t<StdToStringable<T>, std::string> to_string( const T& t ) {
    static_assert( !std::is_same_v<T, std::string> );
    static_assert( !std::is_same_v<T, const char*> );
    static_assert( !std::is_same_v<T, char*> );
    return std::to_string( t );
}

template <class Container, class T = std::decay_t<decltype( *std::declval<Container>().begin() )>>
static typename std::enable_if_t<isContainer<Container>, std::string>
to_string( const Container& container ) {
    std::string str;
    // str += "<" + TYPE_NAME( T ) + ">";
    str += "<" + TYPE_NAME( T() ) + ">";
    str += "[";

    constexpr auto nFirstData = 20;
    constexpr auto nLastData  = 8;
    for ( size_t i = 0; i < container.size(); ++i ) {
        if ( nFirstData < i && i < container.size() - nLastData ) {
            if ( i == container.size() - nLastData - 1 ) { str += "... "; }
            continue;
        }
        const auto& el = container.at( i );

        str += to_string( el );

        if ( i != container.size() - 1 ) str += " ";
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
    // for ( const auto& [key, value] : map ) {
    for ( const auto& pair : map ) {
        str += to_string( pair );
        // str += "{" + to_string( key ) + ": " + to_string( value ) + "}";
        if ( i != map.size() - 1 ) str += ", ";
        ++i;
    }
    str += "]";
    return str;
}

template <Size_t tupleSize, Size_t idx, class... Ts>
static void to_string_recurse( const std::tuple<Ts...>& tuple, std::string& str ) {
    // str += ", " + typeName( T() );
    str += ", " + to_string( std::get<idx>( tuple ) );
    if constexpr ( idx + 1 < tupleSize ) {
        to_string_recurse<tupleSize, idx + 1, Ts...>( tuple, str );
    }
}

template <class... Ts>
static std::string to_string( const std::tuple<Ts...>& tuple ) {
    // std::string str = "tuple<";
    static_assert( sizeof...( Ts ) > 0 );

    std::string str = "(";
    str += to_string( std::get<0>( tuple ) );
    if constexpr ( sizeof...( Ts ) > 1 ) {
        to_string_recurse<sizeof...( Ts ), 1, Ts...>( tuple, str );
        // to_string_recurse<2, 0, Ts...>(tuple, str);
    }
    str += ")";
    return str;
    // return "map<" + typeName( Key() ) + ", " + typeName( Value() ) +
    // ">";
}

template <typename T>
using hubToStringable_t = decltype( hub::to_string( std::declval<T>() ) );

template <typename T, typename = std::void_t<>>
struct hubToStringable : std::false_type {};

template <typename T>
struct hubToStringable<T, std::void_t<hubToStringable_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool hubToStringable_v = hubToStringable<T>::value;

// template <class T>
// typename std::enable_if_t<! std::is_same_v<T, std::string> && ! std::is_same_v<std::remove_pointer_t<T>,const char> && hubToStringable_v<T>, std::ostream&> operator<<( std::ostream& os,
//                                                                            const T& t ) {
//     os << hub::to_string( t );
//     return os;
// }
// template <class T>
// typename std::enable_if_t<! std::is_same_v<T, std::string> && ! std::is_same_v<std::remove_pointer_t<T>,const char> && hubToStringable_v<T>, std::ostream&> operator<<( std::ostream& os,
                                                                           // const T& t ) {
    // os << hub::to_string( t );
    // return os;
// }


// template <class T, class U>
// std::ostream& operator<<( std::ostream& os, const std::pair<T, U>& pair ) {
//     os << toString( pair );
//     return os;
// }

// template <class Container, class T = std::decay_t<decltype( *std::declval<Container>().begin()
// )>> #if CPP_VERSION >= 20
//     requires( !std::is_same_v<Container, std::string> )
// std::ostream&
// #else
// typename std::enable_if_t<!std::is_same_v<Container, std::string>, std::ostream&>
// #endif
// operator<<( std::ostream& os, const Container& container ) {
//     os << toString( container );
//     return os;
// }

// template <class T>
//     // requires hasToString<T>
// typename std::enable_if_t<hasToString<T>, std::ostream&>
// operator<<( std::ostream& os, const T& t ) {
//     os << t.toString();
//     // os << toString(t);
//     return os;
// }

// namespace hub {

// #if ( __cplusplus < 201703L )
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

#if defined( OS_MACOS ) && CPP_VERSION <= 14 // std::void_t not supported by AppleClang (c++14)
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

// #if CPP_VERSION >= 20 // concept
// template <class T>
// concept Printable = requires( T a ) { std::cout << a; };
// #else
// template <class T>
// static constexpr auto Printable = true;
// #endif

//} // namespace hub

} // namespace hub

// template <class T>
// typename std::enable_if_t<! std::is_same_v<T, std::string> && hub::hubToStringable_v<T>, std::ostream&> operator<<( std::ostream& os,
//                                                                            const T& t ) {
//     os << hub::to_string( t );
//     return os;
// }

template <class T>
typename std::enable_if_t<hub::hasToString<T>, std::ostream&>
operator<<( std::ostream& os, const T& t ) {
    os << t.toString();
    return os;
}

template <class T>
std::ostream& operator<<( std::ostream& os, const std::vector<T>& vector ) {
    os << hub::to_string( vector );
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



//     // constexpr bool stringable   = requires( T t ) { str += t; } &&
//     !std::is_arithmetic_v<T>;
//     // constexpr bool hasToString = requires( T t ) { t.toString(); };
//         // if constexpr ( stringable ) { str += el; }
//         // else if constexpr ( hasToString ) { str += el.toString(); }
//         // else { str += std::to_string( el ); }
//     return "string";

// template <typename TupleT, std::size_t... Is>
// std::ostream& printTupleImp( std::ostream& os, const TupleT& tp, std::index_sequence<Is...> ) {
// template <class Tuple, class... Args>
// static auto to_string( const std::tuple<Args...>& tuple ) {
//     size_t index   = 0;
//     std::string str;
//     auto printElem = [&index, &str]( const auto& x ) {
//         if ( index++ > 0 ) str += ", ";
//         str += TYPE_NAME(x) + ":" + x;
//     };

//     str += "(";
//     // ( printElem( std::get<Is>( tp ) ), ... );
//     str += ")";
//     return str;
//     // return os;
// }

// template <class Tuple, class T = std::decay_t<decltype( std::get<0>( std::declval<Tuple>() ) )>>
// typename std::enable_if_t<std::is_same_v<Tuple, std::tuple<T>>, std::string>
// to_string( const Tuple& tuple ) {
//     // return "(" + to_string( std::get<0>( tuple ) ) + ")";
//     return "tuple";
// }

// template <class Tuple,
//           class T = std::decay_t<decltype( std::get<0>( std::declval<Tuple>() ) )>,
//           class U = std::decay_t<decltype( std::get<1>( std::declval<Tuple>() ) )>>
// typename std::enable_if_t<std::is_same_v<Tuple, std::tuple<T, U>>, std::string>
// to_string( const Tuple& tuple ) {
//     // return "(" + to_string( std::get<0>( tuple ) ) + ", " + to_string( std::get<1>( tuple ) )
//     + ")"; return "tuple2";
// }

// template <class Tuple,
//           class T = std::decay_t<decltype( std::get<0>( std::declval<Tuple>() ) )>,
//           class U = std::decay_t<decltype( std::get<0>( std::declval<Tuple>() ) )>,
//           class V = std::decay_t<decltype( std::get<1>( std::declval<Tuple>() ) )>>
// typename std::enable_if_t<std::is_same_v<Tuple, std::tuple<T, U, V>>, std::string>
// to_string( const Tuple& tuple ) {
//     return "tuple3";
//     // return "(" + to_string( std::get<0>( tuple ) ) + ", " + to_string( std::get<1>( tuple ) )
//     +
//            // ", " + to_string( std::get<2>( tuple ) ) + ")";
// }
