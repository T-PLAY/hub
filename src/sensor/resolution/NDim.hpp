
#pragma once

// #include <numeric>
// #include <string>
// #include <algorithm>
// #include <any>
// #include <sstream>
 #include <vector>
//#include <type_traits>

#include "core/Macros.hpp"
// #include "core/Serial.hpp"
// #include "core/Traits.hpp"
// #include "core/Vector.hpp"

// #include "Format.hpp"

// #include "io/input/Input.hpp"
// #include "io/output/Output.hpp"

// #include "core/Input.hpp"

namespace hub {
namespace sensor {
namespace resolution {

//class NDim
//{

//  public:
//    //    template <class... Ns>
//    //    constexpr NDim( Ns&&... ns ) : ns { std::forward<Size_t>( ns )... } {}

//    //    NDim(NDim && nDim) = default;
//    template <class Serial>
//    void serialize( Serial& serial ) {
//        serial( ns );
//    }


//    constexpr NDim() {
////        ns[0] = 1;
//        ns.push_back(1);
//    }

//    template <class... Ns>
//    //    requires( ( Ns > 0 ) && ... )
//    constexpr NDim( Ns&&... ns_ )
//        : ns { std::forward<Size_t>( ns_ )... }
//    {
////        for (int i = 0; i < )
//        //        static_assert(((ns_ > 0) && ...));
//#ifdef DEBUG
//        for ( int i = 0; i < sizeof...( ns_ ); ++i ) {
//            assert(i != 0 || ns[i] > 0);
//            assert(i == 0 || ns[i] > 1);
////            for (const auto n_ : {ns_...}) {
////            static_assert((Size_t)n_ > 0);

////            }
//        }
//#endif
//    }

//    constexpr auto size() const {
//        return ns.size();
////        auto ret = 0;
////        while ( ret < s_nMax && ns[ret] != 0 ) {
////            ++ret;
////        }
////        assert(ret > 0);
////        return ret;
//    }

//    //    NDim() = delete;
//    //    NDim() = default;
//    //    NDim(const NDim & nDim) = default;

//    constexpr auto capacity() const {

//        Size_t ret = 1;
//        for (const auto & n : ns) {
////        for ( int i = 0; i < size(); ++i ) {
////            const auto& n = ns[i];
//            ret *= n;
//        }
//        assert(ret > 0);
//        return ret;
//    }

//    constexpr bool operator==( const NDim& nDim ) const {
//        return ns == nDim.ns;
////        bool ret = true;
////        for ( int i = 0; i < s_nMax; ++i ) {
////            if ( ns[i] != nDim.ns[i] ) return false;
////        }
////        return ret;
//        //        return ns == nDim.ns;
//    }

//    SRC_API friend std::ostream& operator<<( std::ostream& os, const NDim& nDim );

////  private:
////    static constexpr auto s_nMax = 5;

//  public:
//    //    const std::array<Size_t, s_nMax> ns;
//    std::vector<Size_t> ns;
////    int size = 0;
////    Size_t ns[s_nMax] = { 0 };
//};
//static_assert( sizeof( NDim ) == 20 );

// template <Size_t N>
// class Dims1D : public NDim
//{
//   public:

//    constexpr auto n() { return ns[0]; }
//};

// namespace nDim {
//  static constexpr NDim Full_HD { 1920, 1080 };
//   NDim nDim = Full_HD;
//   NDim dims2 = nDim;
//  static constexpr NDim Full_HD_PLUS { 2160, 1080 };
//  static constexpr NDim Ultrawide { 2560, 1080 };
// } // namespace nDim

///////////////////////////////////////// TEMPLATE

// template <Size_t... Ns>
// class NDimTI
//{
//   public:
//     static constexpr auto size() { return sizeof...( Ns ); }

////    template <Size_t iDim>
////    static constexpr Size_t get() {
////        static_assert( 0 <= iDim && iDim < size() );
////        int i = 0;
////        for ( const auto& n : { Ns... } ) {
////            if ( i == iDim ) return n;
////            ++i;
////        }
////        return 0;
////    }

//    static constexpr auto capacity() {
//        Size_t ret = 1;
//        for ( const auto& _n : { Ns... } ) {
//            ret *= _n;
//        }
//        return ret;
//    }

//    template <class Output>
//    static void write( Output& output ) {
//        output.write( NDim { Ns... } );
//    }
////    static constexpr bool readable = false;
//    static struct {} notReadable;
////    [[deprecated]] void read();

////    virtual void notReadable() {};

//    //    template <class Output>
//    //    static void write( Output& output ) {
//    //        output.write( NDim { Ns... } ); // packing
//    //    }

//    constexpr bool operator==( const NDim& nDim ) {
//        bool ret = true;
//        if ( size() != nDim.size() ) return false;

//        int i = 0;
//        for ( const auto& n : { Ns... } ) {
//            if ( n != nDim.ns[i] ) return false;
//            ++i;
//        }
//        return ret;
//    }

//    template <Size_t... Ns_>
//    SRC_API friend std::ostream& operator<<( std::ostream& os, const NDimTI<Ns_...>& nDim );
//};

// template <Size_t... Ns>
// std::ostream& operator<<( std::ostream& os, const NDimTI<Ns...>& nDim ) {
//     int i = 0;
//     for ( auto n : { Ns... } ) {
//         os << n;
//         if ( i != nDim.size() - 1 ) os << "x";
//         ++i;
//     }
//     return os;
// }

///////////////////////////////////////////////////////////

// template <Size_t... Ns>
// class NDimT : public NDimTI<Ns...>
//{
//   public:
//     template <Size_t iDim>
//     static constexpr Size_t n() {
//         static_assert( 0 <= iDim && iDim < NDimTI<Ns...>::size() );
//         int i = 0;
//         for ( const auto& n : { Ns... } ) {
//             if ( i == iDim ) return n;
//             ++i;
//         }
//         return 0;
//     }
// };

// template <Size_t N>
// class NDimT<N> : public NDimTI<N>
//{
//   public:
//     static constexpr auto n() { return N; }
// };

// template <Size_t N, Size_t N2>
// class NDimT<N, N2> : public NDimTI<N, N2>
//{
//   public:
//     static constexpr Size_t width() { return N; }
//     static constexpr Size_t height() { return N2; }
// };

// template <Size_t N, Size_t N2, Size_t N3>
// class NDimT<N, N2, N3> : public NDimTI<N, N2, N3>
//{
//   public:
//     static constexpr Size_t nx() { return N; }
//     static constexpr Size_t ny() { return N2; }
//     static constexpr Size_t nz() { return N3; }
// };

// namespace nDim {

// using Full_HD      = NDimT<1920, 1080>;
// using Full_HD_Plus = NDimT<2160, 1080>;
// using UltraWide    = NDimT<2560, 1080>;

//} // namespace nDim

} // namespace resolution
} // namespace sensor
} // namespace hub
