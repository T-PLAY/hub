
#pragma once

// #include <numeric>
// #include <string>
//#include <algorithm>
//#include <any>
//#include <sstream>
//#include <vector>

#include "core/Macros.hpp"
//#include "core/Serial.hpp"
//#include "core/Traits.hpp"
//#include "core/Vector.hpp"

// #include "Format.hpp"

// #include "io/input/Input.hpp"
// #include "io/output/Output.hpp"

//#include "core/Input.hpp"

namespace hub {
namespace sensor {
namespace resolution {

class Dims
{

  public:
    //    template <class... Ns>
    //    constexpr Dims( Ns&&... ns ) : ns { std::forward<Size_t>( ns )... } {}

    //    Dims(Dims && dims) = default;

    constexpr auto size() const {
        auto ret = 0;
        while ( ret < s_nMax && ns[ret] != 0 ) {
            ++ret;
        }
        return ret;
    }

    //    Dims() = delete;
    //    Dims() = default;
    //    Dims(const Dims & dims) = default;

    constexpr auto capacity() const {

        Size_t ret = 1;
        for ( int i = 0; i < size(); ++i ) {
            const auto& n = ns[i];
            ret *= n;
        }
        return ret;
    }

    constexpr bool operator==( const Dims& dims ) const {
        bool ret = true;
        for ( int i = 0; i < s_nMax; ++i ) {
            if ( ns[i] != dims.ns[i] ) return false;
        }
        return ret;
        //        return ns == dims.ns;
    }

    SRC_API friend std::ostream& operator<<( std::ostream& os, const Dims& dims );

  private:
    static constexpr auto s_nMax = 5;

  public:
    //    const std::array<Size_t, s_nMax> ns;
    Size_t ns[s_nMax] = { 0 };
};
static_assert( sizeof( Dims ) == 20 );

// template <Size_t N>
// class Dims1D : public Dims
//{
//   public:

//    constexpr auto n() { return ns[0]; }
//};

namespace dims {
// static constexpr Dims Full_HD { 1920, 1080 };
//  Dims dims = Full_HD;
//  Dims dims2 = dims;
// static constexpr Dims Full_HD_PLUS { 2160, 1080 };
// static constexpr Dims Ultrawide { 2560, 1080 };
} // namespace dims

///////////////////////////////////////// TEMPLATE

template <Size_t... Ns>
class IDimsT
{
  public:
    static constexpr auto size() { return sizeof...( Ns ); }

//    template <Size_t iDim>
//    static constexpr Size_t get() {
//        static_assert( 0 <= iDim && iDim < size() );
//        int i = 0;
//        for ( const auto& n : { Ns... } ) {
//            if ( i == iDim ) return n;
//            ++i;
//        }
//        return 0;
//    }

    static constexpr auto capacity() {
        Size_t ret = 1;
        for ( const auto& _n : { Ns... } ) {
            ret *= _n;
        }
        return ret;
    }

    template <class Output>
    static void write( Output& output ) {
        output.write( Dims { Ns... } );
    }
//    static constexpr bool readable = false;
    static struct {} notReadable;
//    [[deprecated]] void read();

//    virtual void notReadable() {};

    //    template <class Output>
    //    static void write( Output& output ) {
    //        output.write( Dims { Ns... } ); // packing
    //    }

    constexpr bool operator==( const Dims& dims ) {
        bool ret = true;
        if ( size() != dims.size() ) return false;

        int i = 0;
        for ( const auto& n : { Ns... } ) {
            if ( n != dims.ns[i] ) return false;
            ++i;
        }
        return ret;
    }

    template <Size_t... Ns_>
    SRC_API friend std::ostream& operator<<( std::ostream& os, const IDimsT<Ns_...>& dims );
};

template <Size_t... Ns>
std::ostream& operator<<( std::ostream& os, const IDimsT<Ns...>& dims ) {
    int i = 0;
    for ( auto n : { Ns... } ) {
        os << n;
        if ( i != dims.size() - 1 ) os << "x";
        ++i;
    }
    return os;
}

/////////////////////////////////////////////////////////

template <Size_t... Ns>
class DimsT : public IDimsT<Ns...>
{
  public:
    template <Size_t iDim>
    static constexpr Size_t n() {
        static_assert( 0 <= iDim && iDim < IDimsT<Ns...>::size() );
        int i = 0;
        for ( const auto& n : { Ns... } ) {
            if ( i == iDim ) return n;
            ++i;
        }
        return 0;
    }
};

template <Size_t N>
class DimsT<N> : public IDimsT<N>
{
  public:
    static constexpr auto n() { return N; }
};

template <Size_t N, Size_t N2>
class DimsT<N, N2> : public IDimsT<N, N2>
{
  public:
    static constexpr Size_t width() { return N; }
    static constexpr Size_t height() { return N2; }
};

template <Size_t N, Size_t N2, Size_t N3>
class DimsT<N, N2, N3> : public IDimsT<N, N2, N3>
{
  public:
    static constexpr Size_t nx() { return N; }
    static constexpr Size_t ny() { return N2; }
    static constexpr Size_t nz() { return N3; }
};

namespace dims {

using Full_HD      = DimsT<1920, 1080>;
using Full_HD_Plus = DimsT<2160, 1080>;
using UltraWide    = DimsT<2560, 1080>;

} // namespace dims

} // namespace resolution
} // namespace sensor
} // namespace hub
