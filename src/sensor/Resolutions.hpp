
#pragma once

// #include <numeric>
// #include <string>
// #include <algorithm>
// #include <any>
// #include <sstream>
// #include <vector>
#include <functional>
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

// template <typename... ResolutionT>
 template <class... Resolutions>
//template <ResolutionT... Resolutions>
 class ResolutionsT
{
  public:
//    static constexpr Resolutions getResolutions() { return { Ts::getResolution()... }; }
//    static constexpr Size_t depth = 1 + std::max( 0u, Ts::depth... );
//    struct internal {
//        decltype...(Ts);
//    };

//    static constexpr Size_t depth() {
//        return std::max(Ts::depth()...);
//    }

    template <class Output>
    static void write( Output& output ) {
//        output.write( getResolutions() );
    }

    static struct {
    } notReadable;

    static constexpr auto nByte() {
        auto size = 0;
        int _[]   = { ( size = size + Resolutions::nByte() )... };
        return size;
    }
    using Tuple = std::tuple<Resolutions...>;
    //    static constexpr auto Size = sizeof...(Resolutions);
    static constexpr auto nResolution = sizeof...( Resolutions );
    //    using Nth = typename std::tuple_element<N, Tuple>::type;
//    template <Size_t ith>
//    using get = typename std::tuple_element<ith, Tuple>::type;
    //    using First = Nth<0>;
    //    using Last = Nth<Size - 1>;

    template <Size_t i>
    using getResolution = typename std::tuple_element<i, Tuple>::type;

    template <class Type>
//    using has = (Resolutions::template has<Type>() || ...);
    static constexpr auto hasType() {
//        (Resolutions::nByte() + ...);
        return (std::is_same<Type, decltype(Resolutions::type())>() || ...);
    }

    template <class... Types>
    requires(sizeof...(Types) > 1)
    static constexpr auto hasType() {
        return (hasType<Types>() && ...);
    }

//    template <class Type>
//        requires(has<Type>())
//    static constexpr auto get() {
//        for (auto res : {Resolutions()...}) {
//            if (std::is_same<decltype(res.type()), Type>()) {
//                return res;
//            }
//        }
//    }


    //    static constexpr auto nResolution = Size;

    //    static constexpr auto nResolution() {
    //        auto size = 0;
    //        int _[]   = { ( size = size + Resolutions::nResolution() )... };
    //        return size;
    //    }
    //    static auto name() {
    //        std::string str;
    ////    ( os << ... << Resolutions() );
    //        std::strstream sstream;
    //        ( sstream << ... << Resolutions() );
    //        return sstream.str();
    //    }
    static constexpr std::string name() {
//        std::string str = "<";
        std::string str;
        int i = 0;
        for (auto n : {Resolutions::name()...}) {
            str += n;
            if (i != sizeof...(Resolutions) - 1)
                str += "_";
            ++i;
        }
//        str += ( Resolutions::name() + ... );
        //        if constexpr ( requires { FormatT::name(); } ) { str += FormatT::name(); }
        //        else { str += TYPE_NAME( FormatT ); }

        //        if ( !( nDim() == 1 && getDim( 0 ) == 1 ) ) { str += ":" + DimsT::to_string(); }
//        str += ">";
        return str;
    }

    template <class... Ts_>
    SRC_API friend std::ostream& operator<<( std::ostream& os,
                                             const ResolutionsT<Ts_...>& resolutions );

    //        template <class T>
    //        auto& get() const {
    //		    for (auto res : Resolutions...) {

    //		    }
    //    //            return std::get<T>( m_tuple );
    //        }

    //    template <int id, int i, class ResolutionT_, class... ResolutionTs_>
    //    static constexpr auto get_internal(ResolutionT_ resolutionT, ResolutionTs_...
    //    resolutionTs) {
    ////        return ResolutionT2();
    //        if (id == i) return ResolutionT_();
    //        else return get_internal<id, i + 1>(resolutionTs...);
    ////        if constexpr (id == i) return ResolutionT();
    ////        else return ResolutionT2();

    ////        return get_internal<id, i + 1, ResolutionTs_...>();
    //    }

    //    template <int id>
    //    static constexpr auto get() {
    //        return get_internal<id, 0, Resolutions...>();
    //    }

//            template <int id>
//            constexpr auto& get() {
//                return std::get<id>( m_tuple );
//                return std::get<id>( m_tuple );
//            }

//        template <size_t Index = 0, // start iteration at 0 index
//                                    //          typename TTuple,  // the tuple type
//                  //          size_t Size = std::tuple_size_v<std::remove_reference_t<TTuple>>,

////                  Tuple,
//                  //          size
//                  typename TCallable, // the callable to be invoked for each tuple item
//                  typename... TArgs   // other arguments to be passed to the callable
//                  >
//        void for_each( TCallable&& callable, TArgs&&... args ) {
//            if constexpr ( Index < nResolution ) {
//            std::invoke( callable, args..., std::get<Index>( Tuple() ) );

//                if constexpr ( Index + 1 < nResolution )
//                    for_each<Index + 1>( std::forward<TCallable>( callable ),
//                                         std::forward<TArgs>( args )... );
//            }
//        }

    //    auto& at( int i ) {
    //        return true;
    ////        return std::get<i>( m_tuple );
    //    }

    //    constexpr auto& getTuple() { return m_tuple; }

    //    template <class ResolutionT_>
    //    friend std::ostream& operator<<( std::ostream& os, const ResolutionT_& resolution );

    //    template <class Resolution_, class... Resolutions_>
    //    std::ostream& operator<<( std::ostream& os, const Resolution_ & resolution, const
    //    Resolutions_&... resolutions );

    //  private:
    //    int a;
    //    std::tuple<Resolutions...> m_tuple;
};

 template <class... Resolutions>
 std::ostream& operator<<( std::ostream& os, const ResolutionsT<Resolutions...>& resolution) {
     //    (os << resolutions) ...;
     //    (os << ... << resolutions);
     //    if constexpr (sizeof...(Resolutions) > 0)
     //        os << "-";
     os << resolution.name();
//    ( os << ... << Resolutions() );
    //    for (const auto & res : {ResolutionT_ ...}) {
    //    }
    return os;
}

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
