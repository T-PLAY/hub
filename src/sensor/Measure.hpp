
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
#include <tuple>

#include "core/Buffer.hpp"

// #include <span>

// #include "core/Tuple.hpp"

// #include <bits/stdc++.h>

// #include "core/Macros.hpp"
// #include "Format.hpp"
#include "Resolution.hpp"

namespace hub {
namespace sensor {

////////////////////////////////////////////// TEMPLATES
////////////////////////////////////////////////////////////////////////////

template <class ResolutionT>
class MeasureT : public ResolutionT
{
  public:
    using ResolutionT::nByte;
    using Buffer_t = MutableBuffer<nByte()>;

    static constexpr auto getResolutionT() { return ResolutionT(); }

    constexpr MeasureT() = default;

    template <class BufferT>
    MeasureT( BufferT&& buffer_ ) : buffer( std::forward<BufferT>( buffer_ ) ) {}

    void setData( const Buffer_t& buffer_ ) { buffer = buffer_; }

    static struct {
    } notReadable, notWritable;

  private:
    Buffer_t buffer;
};

///////////////////////////////////////////////////////

template <class ResolutionsT>
//template <class... ResolutionTs>
class MeasuresT : public ResolutionsT
{
  public:
    using ResolutionsT::nByte;
    using Buffer_t = MutableBuffer<nByte()>;
//    static constexpr auto nByte() {
//        return ResolutionsT::nByte();
//        return (ResolutionTs::nByte() + ...);
//    }

//    template <class... Args>
//    MeasuresT(Args&&... args)
//    template <class BufferT>
    MeasuresT( const Buffer_t& buffer_ )
        : buffer{buffer_}
//        : buffer( std::forward<BufferT>( buffer_ ) )
    {}

//    MeasuresT(const Buffer_t & buffer)
//        : m_tuple{std::forward<MeasureT<ResolutionTs>>(buffer)...}
//        : m_tuple{std::forward<ResolutionTs...>(args)...}
//        : m_tuple{args...}
//        : m_tuple{std::forward<Args...>(args...)}
//    {
//    }

//    template <int id>
//    auto& get() {
//        //        return std::get<id>( m_tuple );
//        //        return std::get<id>( m_measures );
//        return std::get<id>( m_tuple );
//        //        return MeasuresT::get<id>();
//    }

  private:
//    std::tuple<MeasureT<ResolutionTs>...> m_tuple;
//    std::tuple<ResolutionTs...> m_tuple;
    Buffer_t buffer;
};


// class Measure
//{
//   public:

//    // io
////    static constexpr Size_t ioGetSize() { return sizeof( Measure ) - sizeof( Data_t* ); }
////    const Data_t* ioGetData() const { return (Data_t*)&m_format; }
////    void ioSetData( const Data_t* data, Size_t size ) { memcpy( &m_format, data, size ); }
////    constexpr std::string ioTypeName() const { return "Measure"; }
//    // end io

////    constexpr auto nDim() const { return m_nDim; }

////    constexpr std::string name() const {
////        std::string str = "<";
////        str += m_format.name();
////        str += ":";
////        for ( Size_t i = 0; i < m_nDim; ++i ) {
////            str += std::to_string( m_dims[i] );
////            if ( i != nDim() - 1 ) str += "x";
////            //            ++i;
////        }
////        str += ">";
////        return str;
////    }

//    constexpr auto nByte() const {
//        return m_resolution.nByte();
////        auto size = m_format.nByte();
////        for ( Size_t i = 0; i < m_nDim; ++i ) {
////            size *= m_dims[i];
////        }
////        return size;
//    }

////    Measure( Format format, std::initializer_list<Size_t> nDim ) : m_format( format ) {
////        assert( nDim.size() <= s_maxDim );
////        std::vector<Size_t> dimsVector = nDim;
////        for ( int i = 0; i < dimsVector.size(); ++i ) {
////            m_dims[i] = dimsVector.at( i );
////        }
////        m_nDim = dimsVector.size();
////    }

//    template <typename ResolutionT>
//    Measure( ResolutionT resolution, Data_t * const data )
//        : m_resolution( std::forward<Resolution>(resolution) )
//        , m_data(data)
//    {
//    }

////    Measure() = default;

//    constexpr bool operator==( const Measure& measure ) const {
//        return m_resolution == measure.m_resolution && ! std::memcmp(m_data, measure.m_data,
//        nByte());
////        return nByte() == measure.nByte() && nDim() == measure.nDim();
//    }

////    void setData( Data_t* const data ) {
////        assert( data != nullptr );
////        m_data = data;
////    }
//    void setData( const Data_t* const data, Size_t size ) {
//        assert( m_data != nullptr );
//        assert( nByte() == size );
//        assert( data != nullptr );
//        std::memcpy( m_data, data, size );
//    }

//    SRC_API friend std::ostream& operator<<( std::ostream& os, const Measure& measure );
//    //  private:
//    const Resolution & getResolution() const;

//    Data_t *getData() const;

//  private:
////    static constexpr auto s_maxDim = 3;
//    const Resolution m_resolution;
//    Data_t* const m_data               = nullptr;

////    using nByte = Resolution::nByte;

////    Format m_format;
////    Size_t m_nDim = 0;
//    //    Size_t m_dims[s_maxDim];
//};

// inline Data_t *Measure::getData() const
//{
//     return m_data;
// }

// using Measures = std::vector<Measure>;

////SRC_API friend std::ostream& operator<<( std::ostream& os, const Measures& measures );
// std::ostream& operator<<( std::ostream& os, const Measures& measures );

///**************************************************************************************

// class Measures
//{
//   public:
//     // io
//     static constexpr Size_t ioGetSize() {
//         return sizeof(Size_t) + s_maxMeasure * Measure::ioGetSize();
//     }
//     const Data_t* ioGetData() const {
//         return (Data_t*)&m_nMeasure;
//     }
//     void ioSetData( const Data_t* data, Size_t size ) {
//         memcpy(&m_nMeasure, data, size);
//     }
//     constexpr std::string ioTypeName() const { return "Measures"; }
//     // end io

//    Measures( std::initializer_list<Measure> measures )
////        : m_measures( measures )
//    {
//        assert(measures.size() <= s_maxMeasure);
//        std::vector<Measure> dimsVector = measures;
//        for (int i = 0; i < dimsVector.size(); ++i) {
//            m_measures[i] = dimsVector.at(i);
//        }
//        m_nMeasure = dimsVector.size();
//    }

//    Measures() = default;

//    constexpr auto nByte() const {
//        auto size = 0;
//        for ( const auto& measure : m_measures ) {
//            size += measure.nByte();
//        }
//        return size;
//    }
////    std::vector<Measure>::iterator begin() {
////        return m_measures;
//////        return m_measures.begin();
////    }
////    std::vector<Measure>::iterator end() {
////        return &m_measures[m_nMeasure];
//////        return m_measures.end();
////    }

//    Measure& at( int i ) {
//        assert(i < m_nMeasure);
//        return m_measures[ i ];
//    }

//    const Measure& at( int i ) const {
//        assert(i < m_nMeasure);
//        return m_measures[ i ];
//    }

//    constexpr auto nMeasure() const {
//        return m_nMeasure;
////        return m_measures.size();
//    }

//    constexpr bool operator==( const Measures& measures ) const {
//        if ( nMeasure() == measures.nMeasure() ) {
//            for ( int i = 0; i < nMeasure(); ++i ) {
//                if ( m_measures[ i ] != measures.m_measures[ i ] ) return false;
//            }
//            return true;
//        }
//        return false;
//    }

//    SRC_API friend std::ostream& operator<<( std::ostream& os, const Measures& measures );

//    static constexpr auto s_maxMeasure = 2;
//    Size_t m_nMeasure = 0;
//    Measure m_measures[s_maxMeasure];
////    std::vector<Measure> m_measures;

//};

// template <Format format, Size_t... NDim>
// class MeasureT
//{
//   public:
//     static constexpr auto getFormat() { return format; }
//     static constexpr auto nByte() {
//         auto size = format.nByte;
//         //        size = std::accumulate()
//         for ( auto dim : { NDim... } )
//             size *= dim;
//         return size;
//     }
//     static constexpr auto nDim() {
//         auto nDim = 0;
//         for ( auto n : { NDim... } ) {
//             ++nDim;
//         }
//         return nDim;
//     }
//     static constexpr auto getDim( int iDim ) {
//         auto i = 0;
//         for ( auto dim : { NDim... } ) {
//             if ( i == iDim ) return dim;
//             ++i;
//         }
//         return (Size_t)0;
//     }
//     static constexpr std::string typeName() {
//         std::string str = "<";
//         str += format.name;
//         str += ":";
//         Size_t i = 0;
//         for ( auto dim : { NDim... } ) {
//             str += std::to_string( dim );
//             if ( i != nDim() - 1 ) str += "x";
//             ++i;
//         }
//         //            int _[]  = { ( str += std::to_string(NDim) + ", " )... };
//         str += ">";
//         return str;
//     }
//     static constexpr Size_t nMeasure() { return 1; }

//    constexpr bool operator==( const MeasureT& measure ) const {
//        assert( m_data != nullptr );
//        assert( measure.m_data != nullptr );
//        static_assert( nByte() == measure.nByte() );
//        static_assert( nDim() == measure.nDim() );
//        return !std::memcmp( m_data, measure.m_data, nByte() );
//        //        return m_array == measure.m_array;
//    }

//    const Data_t* getData() const {
//        assert( m_data != nullptr );
//        return m_data;
//        //        return m_array.data();
//    }

//    void setData( const Data_t* const data, Size_t size ) {
//        assert( m_data != nullptr );
//        assert( nByte() == size );

//        assert( data != nullptr );
//        //        data[0] = 0;
//        //        m_data = nullptr;
//        //        m_data[0] = 0;
//        std::memcpy( m_data, data, size );
//    }

//    void setData( Data_t* const data ) {
//        //        assert( getSize() == size );
//        assert( data != nullptr );
//        m_data = data;
//        //        data[0] = 0;
//        //        m_data = nullptr;
//        //        m_data[0] = 0;
//        //        std::memcpy( m_data, data, size );
//    }

//    template <Format formatT, Size_t... DimsT>
//    SRC_API friend std::ostream& operator<<( std::ostream& os,
//                                             const MeasureT<formatT, DimsT...>& measure );

//  private:
//    unsigned char* m_data = nullptr;
//};

// template <Format format, Size_t... NDim>
// std::ostream& operator<<( std::ostream& os, const MeasureT<format, NDim...>& measure ) {

//    os << measure.typeName();
//    os << " = ";

//    const auto* data = measure.m_data;
//    if ( data == nullptr ) { os << "nil"; }
//    else {
//        os << "[";
//        assert( data != nullptr );

//        constexpr Size_t nMaxDataToShow = 10;
//        const auto iMax                   = std::min( measure.nByte(), nMaxDataToShow );
//        for ( auto i = 0; i < iMax; ++i ) {
//            //        os << std::setw( 3 ) << (int)array[i] << " ";
//            os << std::to_string( data[i] );
//            if ( i != iMax - 1 ) os << " ";
//        }
//        if ( measure.nByte() > nMaxDataToShow ) {
//            os << " ... " << std::to_string( data[iMax - 1] );
//        }
//        os << "]";
//    }

//    return os;
//}

//////////////////////////////////////////////////////////////////////////////////

// template <typename... MeasureT>
//// template <Format, Measure<Format format, int... NDim>... Measure>
// class MeasuresT
//{
//   public:
//     static constexpr auto nByte() {
//         auto size = 0;
//         int _[]   = { ( size = size + MeasureT::nByte() )... };
//         return size;
//     }
//     static constexpr auto nMeasure() {
//         auto size = 0;
//         int _[]   = { ( size = size + MeasureT::nMeasure() )... };
//         return size;
//     }

//    template <typename T>
//    auto& get() {
//        return std::get<T>( m_tuple );
//        //        T t;
//        //        m_measures::get<T>();
//        //        return m_measures.get<T>();
//        //        return m_measures.get<T>();
//        //        return std::get<T>( m_measures );
//    }

//    template <int id>
//    auto& get() {
//        //        return std::get<id>( m_tuple );
//        //        return std::get<id>( m_measures );
//        return std::get<id>( m_tuple );
//        //        return MeasuresT::get<id>();
//    }

//    template <size_t Index = 0, // start iteration at 0 index
//                                //          typename TTuple,  // the tuple type
//              //          size_t Size = std::tuple_size_v<std::remove_reference_t<TTuple>>, //
//              tuple
//              //          size
//              typename TCallable, // the callable to be invoked for each tuple item
//              typename... TArgs   // other arguments to be passed to the callable
//              >
//    void for_each( TCallable&& callable, TArgs&&... args ) {
//        if constexpr ( Index < nMeasure() ) {
//            std::invoke( callable, args..., std::get<Index>( m_tuple ) );

//            if constexpr ( Index + 1 < nMeasure() )
//                for_each<Index + 1>( std::forward<TCallable>( callable ),
//                                     std::forward<TArgs>( args )... );
//        }
//    }

//    //    auto& at( int i ) {
//    //        return true;
//    ////        return std::get<i>( m_tuple );
//    //    }

//    constexpr auto& getTuple() { return m_tuple; }

//    template <typename... MeasureT_>
//    SRC_API friend std::ostream& operator<<( std::ostream& os,
//                                             const MeasuresT<MeasureT_...>& measures );

//  private:
//    std::tuple<MeasureT...> m_tuple;
//};

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

// template <typename... MeasureT>
// std::ostream& operator<<( std::ostream& os, const MeasuresT<MeasureT...>& measures ) {
//     os << measures.m_tuple;
//     return os;
// }

} // namespace sensor
} // namespace hub
