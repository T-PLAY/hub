/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/09
	
#pragma once

#include <tuple>
#include <iostream>
#include "Vector.hpp"

template <typename TupleT, std::size_t... Is>
std::ostream& printTupleImp( std::ostream& os, const TupleT& tp, std::index_sequence<Is...> ) {
    size_t index   = 0;
    auto printElem = [&index, &os]( const auto& x ) {
        if ( index++ > 0 ) os << ", ";
        os << TYPE_NAME(x) << ":" << x;
    };

    os << "(";
    ( printElem( std::get<Is>( tp ) ), ... );
    os << ")";
    return os;
}

// template <typename TupleT, std::size_t TupSize = std::tuple_size<TupleT>::value>
// std::ostream& operator<<( std::ostream& os, const TupleT& tp ) {
// //    return os;
//     return printTupleImp( os, tp, std::make_index_sequence<TupSize> {} );
// }

// template <typename TupleT, std::size_t... Is>
// template <typename TupleT, std::size_t Is>
// auto getTupleImpl( const TupleT& tp, int el, std::index_sequence<Is> ) {
//     return std::get<Is>(tp);
////    return (std::get<Is>(tp),...);
////    return el;
////    size_t index   = 0;
////    auto printElem = [&index, &os]( const auto& x ) {
////        if ( index++ > 0 ) os << ", ";
////        os << x;
////    };

////    os << "(";
////    ( printElem( std::get<Is>( tp ) ), ... );
////    os << ")";
////    return os;
//}

// template <typename TupleT, typename El>
// auto tuple_get( const TupleT& tp, const El & el ) {
////    return std::get<el>(tp);
//    return std::get<el>(tp);
////    return getTupleImpl( tp, el, std::make_index_sequence<1> {} );
//}

//template<size_t I = 0, typename... Tp>
//void print(std::tuple<Tp...>& t) {
//    std::cout << std::get<I>(t) << " ";
//    // do things
//    if constexpr(I+1 != sizeof...(Tp))
//        print<I+1>(t);
//}

//#include <iostream>

//#include <functional>
//template <
//    size_t Index = 0, // start iteration at 0 index
//    typename TTuple,  // the tuple type
//    size_t Size =
//    std::tuple_size_v<
//        std::remove_reference_t<TTuple>>, // tuple size
//    typename TCallable, // the callable to be invoked for each tuple item
//    typename... TArgs   // other arguments to be passed to the callable
//    >
//void for_each(TTuple&& tuple, TCallable&& callable, TArgs&&... args)
//{
//    if constexpr (Index < Size)
//    {
//        std::invoke(callable, args..., std::get<Index>(tuple));

//        if constexpr (Index + 1 < Size)
//            for_each<Index + 1>(
//                std::forward<TTuple>(tuple),
//                std::forward<TCallable>(callable),
//                std::forward<TArgs>(args)...);
//    }
//}

//template<int...> struct index_tuple{};

//template<int I, typename IndexTuple, typename... Types>
//struct make_indexes_impl;

//template<int I, int... Indexes, typename T, typename ... Types>
//struct make_indexes_impl<I, index_tuple<Indexes...>, T, Types...>
//{
//    typedef typename make_indexes_impl<I + 1, index_tuple<Indexes..., I>, Types...>::type type;
//};

//template<int I, int... Indexes>
//struct make_indexes_impl<I, index_tuple<Indexes...> >
//{
//    typedef index_tuple<Indexes...> type;
//};

//template<typename ... Types>
//struct make_indexes : make_indexes_impl<0, index_tuple<>, Types...>
//{};


//template<typename Func, typename Last>
//void for_each_impl(Func&& f, Last&& last)
//{
//    f(last);
//}

//template<typename Func, typename First, typename ... Rest>
//void for_each_impl(Func&& f, First&& first, Rest&&...rest)
//{
//    f(first);
//    for_each_impl( std::forward<Func>(f), rest...);
//}

//template<typename Func, int ... Indexes, typename ... Args>
//void for_each_helper( Func&& f, index_tuple<Indexes...>, std::tuple<Args...>&& tup)
//{
//    for_each_impl( std::forward<Func>(f), std::forward<Args>(std::get<Indexes>(tup))...);
//}

//template<typename Func, typename ... Args>
//void for_each( std::tuple<Args...>& tup, Func&& f)
//{
//    for_each_helper(std::forward<Func>(f),
//                     typename make_indexes<Args...>::type(),
//                     std::forward<std::tuple<Args...>>(tup) );
//}

//template<typename Func, typename ... Args>
//void for_each( std::tuple<Args...>&& tup, Func&& f)
//{
//    for_each_helper(std::forward<Func>(f),
//                     typename make_indexes<Args...>::type(),
//                     std::forward<std::tuple<Args...>>(tup) );
//}
