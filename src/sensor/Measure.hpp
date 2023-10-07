
#pragma once

// #include <numeric>
// #include <string>
// #include <vector>
#include <iostream>
// #include <stdlib.h>
// #include <cstdlib>
#include <algorithm>
#include <cstring>
#include <functional>
#include <iomanip>
#include <memory>

// #include <tuple>
// #include "core/Tuple.hpp"

// #include <bits/stdc++.h>
#include <cassert>

// #include "core/Macros.hpp"
#include "Format.hpp"

namespace hub {
namespace sensor {

using DataType = unsigned char;
using SizeType = int;

template <Format format, SizeType... Dims>
//template <typename Format, SizeType... Dims>
//template <typename Format,
class Measure
{
  public:
    static constexpr auto getFormat() {
        return format;
    }
    static constexpr auto getSize() {
//        auto size = format::getSize( format );
        auto size = 1;
//        auto size = format.getSize();
        //        size = std::accumulate()
        for ( auto dim : { Dims... } )
            size *= dim;
        return size;
    }
    static constexpr auto nDim() {
        auto nDim = 0;
        for ( auto n : { Dims... } ) {
            ++nDim;
        }
        return nDim;
    }
    static constexpr auto getDim( SizeType iDim ) {
        SizeType i = 0;
        for ( auto dim : { Dims... } ) {
            if ( i == iDim ) return dim;
            ++i;
        }
        return 0;
    }
    static constexpr std::string typeName() {
        //        return "Measures<" + Measures::typeName() + ">";
//        std::string str = "<" + format::to_string( format ) + ":";
        std::string str = "<";
        str += format.typeName();
        str += ":";
        SizeType i           = 0;
        for ( auto dim : { Dims... } ) {
            str += std::to_string( dim );
            if ( i != nDim() - 1 ) str += "x";
            //            if ( i == iDim ) return dim;
            ++i;
        }
        //            int _[]  = { ( str += std::to_string(Dims) + ", " )... };
        str += ">";
        return str;
    }
    static constexpr SizeType nMeasure() { return 1; }
    //    using Array = std::array<DataType, getSize()>;

    //    template <typename T>
    //    constexpr Measure( T&& t ) : m_array { std::forward<T>( t ) } {}

    //    constexpr Measure() : m_array { 0 } {};
    //    Measure() = default;

    //    explicit Measure( DataType* const data, size_t size ) :
    //        //        m_array { 0 },
    //        m_data( data ) {
    //        assert(getSize() == size);
    ////        std::memset( data, 0, getSize() );
    //    };

    constexpr bool operator==( const Measure& measure ) const {
        assert( m_data != nullptr );
        assert( measure.m_data != nullptr );
        static_assert( getSize() == measure.getSize() );
        static_assert( nDim() == measure.nDim() );
        return !std::memcmp( m_data, measure.m_data, getSize() );
        //        return m_array == measure.m_array;
    }

    //    constexpr const DataType* getData() const {
    //        return m_data;
    //        //        return m_array.data();
    //    }

    DataType* getData() {
        assert( m_data != nullptr );
        return m_data;
        //        return m_array.data();
    }

    void setData( const DataType* const data, size_t len ) {
        assert( m_data != nullptr );
        assert( getSize() == len );

        assert( data != nullptr );
        //        data[0] = 0;
        //        m_data = nullptr;
        //        m_data[0] = 0;
        std::memcpy( m_data, data, len );
    }

    void setData( DataType* const data ) {
        //        assert( getSize() == len );
        assert( data != nullptr );
        m_data = data;
        //        data[0] = 0;
        //        m_data = nullptr;
        //        m_data[0] = 0;
        //        std::memcpy( m_data, data, len );
    }

    //    constexpr auto at( int i ) const {
    //        assert(m_data != nullptr);
    //        return m_data[i];
    //        //        return m_array.at( i );
    //        //            return std::get<id2>(m_tuple);
    //        //        return get<idx>();
    //    }

    //    void setData(unsigned char * data, int size) {
    //        assert(m_data != nullptr);
    //        assert(getSize() == size);
    //        m_data = data;
    //    }

    //    template <typename MeasureT>
    //    template <Format format2, int... Dims2>
    //    SRC_API friend std::ostream& operator<<( std::ostream& os, const Measure<format2,
    //    Dims2...> & measure );

    template <Format formatT, SizeType... DimsT>
    SRC_API friend std::ostream& operator<<( std::ostream& os,
                                             const Measure<formatT, DimsT...>& measure );

  private:
    //    Array m_array;
    unsigned char* m_data = nullptr;
};

//template <class MeasureT>
template <Format format, SizeType... Dims>
std::ostream& operator<<( std::ostream& os, const Measure<format, Dims...>& measure ) {
//    os << "{";
//    const auto& nDim = measure.nDim();
//    for ( int iDim = 0; iDim < nDim; ++iDim ) {
//        //        str += std::to_string( dims[i] );
//        os << measure.getDim( iDim );
//        if ( iDim != nDim - 1 ) {
//            //            str += " x ";
//            os << " x ";
//        }
//    }
//    os << " : " << format << "} = ";

    os << measure.typeName();
    os << " = ";

    //    const auto& array            = measure.m_array;
    const auto* data = measure.m_data;
    if ( data == nullptr ) {
//        os << "undef";
        os << "nil";
    }
    else {
        os << "[";
        assert( data != nullptr );

        constexpr SizeType nMaxDataToShow = 10;
        const auto iMax              = std::min( measure.getSize(), nMaxDataToShow );
        for ( auto i = 0; i < iMax; ++i ) {
            //        os << std::setw( 3 ) << (int)array[i] << " ";
            os << std::to_string(data[i]);
            if ( i != iMax - 1 ) os << " ";
        }
        if ( measure.getSize() > nMaxDataToShow ) { os << " ... " << std::to_string(data[iMax - 1]); }
        os << "]";
    }

    return os;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename... Measure>
// template <Format, Measure<Format format, int... Dims>... Measure>
class Measures
{
  public:
    static constexpr SizeType getSize() {
        SizeType size = 0;
        int _[]  = { ( size = size + Measure::getSize() )... };
        return size;
    }
    static constexpr SizeType nMeasure() {
        SizeType size = 0;
        int _[]  = { ( size = size + Measure::nMeasure() )... };
        return size;
    }

    //    Measures( DataType* const data, size_t size )  {
    //        assert(data != nullptr);
    //        assert(size == getSize());

    //    template <typename... T>
    //    constexpr Measures( T&&... t ) : m_tuple { std::forward<T>( t )... } {}

    //    Measures() = default;

    //    constexpr bool operator==( const Measures& measures ) const {
    //        static_assert(getSize() == measures.getSize());
    //        static_assert(nMeasure() == measures.nMeasure());
    //        return m_tuple == measures.m_tuple;
    //    }

    //    template <typename T>
    //    constexpr auto& get() const {
    //        return std::get<T>( m_tuple );
    //    }

    //    template <typename T>
    //    auto & get() {
    //        return std::get<T>( m_tuple );
    //    }

    //    template <int id>
    //    constexpr auto & get() const {
    //        return std::get<id>( m_tuple );
    //    }

    //    template <int id>
    //    constexpr auto& get() {
    //        return std::get<id>( m_tuple );
    //    }

    //    template <typename TCallable> // the callable to be invoked for each tuple item
    //    void for_each( TCallable&& callable ) {
    //        tuple_for_each(m_tuple, callable);
    ////                                 std::forward<TArgs>( args )... );
    //    }
    template <size_t Index = 0, // start iteration at 0 index
                                //          typename TTuple,  // the tuple type
              //          size_t Size = std::tuple_size_v<std::remove_reference_t<TTuple>>, // tuple
              //          size
              typename TCallable, // the callable to be invoked for each tuple item
              typename... TArgs   // other arguments to be passed to the callable
              >
    void for_each( TCallable&& callable, TArgs&&... args ) {
        if constexpr ( Index < nMeasure() ) {
            std::invoke( callable, args..., std::get<Index>( m_tuple ) );

            if constexpr ( Index + 1 < nMeasure() )
                for_each<Index + 1>( std::forward<TCallable>( callable ),
                                     std::forward<TArgs>( args )... );
        }
    }

    //    auto& at( int i ) {
    //        return true;
    ////        return std::get<i>( m_tuple );
    //    }

    //    constexpr auto& getTuple() { return m_tuple; }

    template <typename... MeasureT>
    SRC_API friend std::ostream& operator<<( std::ostream& os,
                                             const Measures<MeasureT...>& measures );

  private:
    std::tuple<Measure...> m_tuple;
};

// template <typename TupleT, >

// template <typename TupleT, std::size_t... Is>
// std::ostream& printTupleImp( std::ostream& os, const TupleT& tp, std::index_sequence<Is...> ) {
//     size_t index   = 0;
//     auto printElem = [&index, &os]( const auto& x ) {
//         if ( index++ > 0 ) os << ", ";
//         os << x;
//     };

//    os << "(";
//    ( printElem( std::get<Is>( tp ) ), ... );
//    os << ")";
//    return os;
//}

// template <typename TupleT, std::size_t TupSize = std::tuple_size<TupleT>::value>
// std::ostream& operator<<( std::ostream& os, const TupleT& tp ) {
//     return printTupleImp( os, tp, std::make_index_sequence<TupSize> {} );
// }

// #include "core/Tuple.hpp"

template <typename TupleT, std::size_t... Is>
std::ostream& printTupleImp( std::ostream& os, const TupleT& tp, std::index_sequence<Is...> ) {
    size_t index   = 0;
    auto printElem = [&index, &os]( const auto& x ) {
        if ( index++ > 0 ) os << ", ";
        os << x;
    };

    os << "(";
    ( printElem( std::get<Is>( tp ) ), ... );
    os << ")";
    return os;
}

template <typename TupleT, std::size_t TupSize = std::tuple_size<TupleT>::value>
std::ostream& operator<<( std::ostream& os, const TupleT& tp ) {
    return printTupleImp( os, tp, std::make_index_sequence<TupSize> {} );
}

//template <size_t Index = 0, // start iteration at 0 index
//          typename TTuple,  // the tuple type
//          size_t Size = std::tuple_size_v<std::remove_reference_t<TTuple>>, // tuple size
//          typename TCallable, // the callable to be invoked for each tuple item
//          typename... TArgs   // other arguments to be passed to the callable
//          >
//void tuple_for_each( TTuple&& tuple, TCallable&& callable, TArgs&&... args ) {
//    if constexpr ( Index < Size ) {
//        std::invoke( callable, args..., std::get<Index>( tuple ) );

//        if constexpr ( Index + 1 < Size )
//            tuple_for_each<Index + 1>( std::forward<TTuple>( tuple ),
//                                       std::forward<TCallable>( callable ),
//                                       std::forward<TArgs>( args )... );
//    }
//}

template <typename... MeasureT>
std::ostream& operator<<( std::ostream& os, const Measures<MeasureT...>& measures ) {
    //    for_each(measures.m_tuple, [&os](const auto & measure) {
    //        os << measure;
    //    });
    os << measures.m_tuple;
    return os;
}

} // namespace sensor
} // namespace hub
