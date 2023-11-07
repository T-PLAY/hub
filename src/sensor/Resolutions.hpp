
#pragma once

// #include <numeric>
// #include <string>
// #include <algorithm>
// #include <any>
// #include <sstream>
// #include <vector>
#include <type_traits>
#include <cmath>
// #include <strstream>
// #include <strstream>

// #include "core/Macros.hpp"
//  #include "core/Serial.hpp"
// #include "core/Traits.hpp"
//  #include "core/Vector.hpp"
//  #include "core/Tuple.hpp"

// #include "resolution/NDim.hpp"
// #include "resolution/Format.hpp"

// #include "io/input/Input.hpp"
// #include "io/output/Output.hpp"

// #include "core/Input.hpp"
#include "Resolution.hpp"

namespace hub {
namespace sensor {

//using Resolutions = std::vector<Resolution>;

//////////////////////////////////////////// TEMPLATES /////////////////////////////////////////


// template <class... FormatT>
// constexpr auto make_Resolution() {
//     return ResolutionsT<FormatT...>();
// }

// template <Size_t N, class... FormatT>
// constexpr auto make_Resolution() {
//     return Resolution1DT<N, FormatT...>();
// }

// template <class... ResolutionTs>
// class ResolutionsT

// template <class ResolutionsT, Size_t N = 1, Size_t... NDimTs>
//     requires( N > 1 && ( ( NDimTs > 1 ) && ... ) )
// class ResolutionsPackT
//{
//   public:
//     //    static constexpr auto nByte() {
//     //        auto size = ResolutionsT::nByte() * N;
//     //        for (auto n : {NDimTs...}) {
//     //            size *= n;
//     //        }
//     //        return size;
//     //    }
//     static constexpr auto nByte() {
//         auto size = ResolutionsT::nByte() * N;
//         if constexpr ( sizeof...( NDimTs ) > 0 ) {
//             for ( auto dim : { NDimTs... } ) {
//                 size *= dim;
//             }
//         }
//         return size;
//     }
//     static_assert( nByte() > 0 );

//    template <class ResolutionsT_, Size_t N_, Size_t... NDimTs_>
//    SRC_API friend std::ostream&
//    operator<<( std::ostream& os,
//                const ResolutionsPackT<ResolutionsT_, N_, NDimTs_...>& resolutions );
//};

// template <class ResolutionsT_, Size_t N_, Size_t... NDimTs_>
// SRC_API std::ostream&
// operator<<( std::ostream& os, const ResolutionsPackT<ResolutionsT_, N_, NDimTs_...>& resolutions
// ) {

//    constexpr auto nDim = 1 + sizeof...( NDimTs_ );

//    if ( !( nDim == 1 && N_ == 1 ) ) {
//        Size_t i = 0;
//        os << N_;
//        //        ( os << ... << "x" << NDimTs_ );
//        if constexpr ( sizeof...( NDimTs_ ) > 0 ) {
//            for ( auto dim : { NDimTs_... } ) {
//                os << dim;
//                if ( i != nDim - 1 ) os << "x";
//                ++i;
//            }
//        }
//        os << ":";
//        os << ResolutionsT_();
//    }
//    return os;
//}

// template <class... ResolutionTs>
// constexpr Resolutions ResolutionsT<ResolutionTs...>::getResolutions() {
////    Resolutions resolutions;
//    Resolutions resolutions({ResolutionTs::getResolution()...});
//    return resolutions;
//}

// template <typename TupleT, std::size_t... Is>
// std::ostream& printTupleImp( std::ostream& os, const TupleT& tp, std::index_sequence<Is...> ) {
//     size_t index   = 0;
//     auto printElem = [&index, &os]( const auto& x ) {
//         if ( index++ > 0 ) os << "-";
//         os << x;
//     };

////    if ( sizeof...( Is ) > 1 ) os << "<";
//    ( printElem( std::get<Is>( tp ) ), ... );
////    if ( sizeof...( Is ) > 1 ) os << ">";
//    return os;
//}

// template <typename TupleT, std::size_t TupSize = std::tuple_size<TupleT>::value>
// std::ostream& operator<<( std::ostream& os, const TupleT& tp ) {
//     return printTupleImp( os, tp, std::make_index_sequence<TupSize> {} );
// }

// template <class ResolutionTs>
// std::ostream& operator<<( std::ostream& os, const ResolutionTs& resolution ) {
////    (os << resolutions) ...;
////    os << resolution;
////    (os << ... << resolutions);
//    (os << ResolutionTs());
//    return os;
//}

// template <class... ResolutionTs>
// std::ostream& operator<<( std::ostream& os, const ResolutionTs&... resolutions ) {

// class Resolution
//{
//   public:
//     Format format;
//     resolution::NDim nDim;

//    // io
//    //    static constexpr Size_t ioGetSize() { return sizeof( Measure ) - sizeof( Data_t* ); }
//    //    const Data_t* ioGetData() const { return (Data_t*)&m_format; }
//    //    void ioSetData( const Data_t* data, Size_t size ) { memcpy( &m_format, data, size ); }
//    //    constexpr std::string ioTypeName() const { return "Measure"; }
//    // end io

//    Resolution() = default;

//  public:
//    constexpr auto nByte() const {
//        //        auto size = m_format.nByte;
//        auto size = format.nByte;
//        for ( int i = 0; i < nDim.size(); ++i ) {
//            size *= nDim.ns[i];
//        }
//        return size;
//    }

//    constexpr bool operator==( const Resolution& resolution ) const {
//        return format == resolution.format && nDim == resolution.nDim;
//        //        return getFormat() == resolution.getFormat() && getDims() ==
//        resolution.getDims();
//    }

//    SRC_API friend std::ostream& operator<<( std::ostream& os, const Resolution& resolution );
//};

// template <class... Ns>
// class ResolutionT
//{
//   public:
//     const Format format;
//     ResolutionT() = delete;

//    constexpr ResolutionT( Format format, Ns&&... ns ) :
//        format { format }
//    {}

//    constexpr auto getFormat() const { return format; }

//    constexpr auto nByte() const {
//        int size = format.nByte;
//        return size;
//    }

//};

/////////////////////////////////////////////////////////////

// using Resolutions = std::vector<Resolution>;

// std::ostream& operator<<( std::ostream& os, const Resolutions& resolutions );
// std::string toString( const Resolutions& resolutions );
// constexpr Size_t nByte( const Resolutions& resolutions );

// constexpr Size_t nByte( const Resolutions& resolutions ) {
//     Size_t size = 0;
//     for ( const auto& resolution : resolutions ) {
//         size += resolution.nByte();
//     }
//     return size;
// }

////////////////////////////////////////// TEMPLATE /////////////////////////////////////

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

} // namespace sensor
} // namespace hub
