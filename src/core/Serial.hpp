#pragma once

#include <iostream>

namespace hub {

template <typename T>
//    using readable_t = decltype( std::declval<T>().read( std::declval<Input&>() ) );
using serializable_t = decltype( std::declval<T&>().serialize() );

template <typename T, typename = std::void_t<>>
struct serializable : std::false_type {};

template <typename T>
struct serializable<T, std::void_t<serializable_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool serializable_v = serializable<T>::value;

//template <typename T>
//static constexpr bool serializable_v = std::enable_if<T::serialize>::value;

template <class... Ts>
class Serial
{
  public:
    //    template <class... Ts>
    Serial( Ts&&... ts ) : m_ts( std::forward<Ts&>( ts )... ) {}

    //    void print() {
    //        for (const auto & t : m_ts) {
    //            std::cout << t << std::endl;
    //        }
    //    }

    void print() { print( m_ts ); }

    template <std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I == sizeof...( Tp ), void>::type print( std::tuple<Tp...>& t ) {
    }

    template <std::size_t I = 0, typename... Tp>
        inline typename std::enable_if <
        I<sizeof...( Tp ), void>::type print( std::tuple<Tp...>& t ) {
        std::cout << std::get<I>( t ) << std::endl;
        print<I + 1, Tp...>( t );
    }

    //  private:
    std::tuple<Ts...> m_ts;
};

template <class... Ts>
auto make_serial( Ts&&... ts ) {
    return Serial<Ts&&...>( ts... );
}

//template <typename TupleT, std::size_t... Is>
//std::ostream& printTupleImp( std::ostream& os, const TupleT& tp, std::index_sequence<Is...> ) {
//    size_t index   = 0;
//    auto printElem = [&index, &os]( const auto& x ) {
//        if ( index++ > 0 ) os << ", ";
//        os << x;
//    };

//    os << "(";
//    ( printElem( std::get<Is>( tp ) ), ... );
//    os << ")";
//    return os;
//}

//template <typename TupleT, std::size_t TupSize = std::tuple_size<TupleT>::value>
//std::ostream& operator<<( std::ostream& os, const TupleT& tp ) {
//    return printTupleImp( os, tp, std::make_index_sequence<TupSize> {} );
//}


} // namespace hub
