
#pragma once

#include <type_traits>

#include "Format.hpp"
#include "core/Macros.hpp"

namespace hub {
namespace sensor {

using NDim = std::vector<Size_t>;

class Resolution
{
  public:
    Format format;
    //    Format format;
    NDim nDim;

    //    template <class Serial>
    //    void serialize( Serial& serial ) {
    //        serial(nDim);
    //    }

    bool operator==( const Resolution& other ) const {
        return format == other.format && nDim == other.nDim;
    }

    SRC_API friend std::ostream& operator<<( std::ostream& os, const Resolution& resolution );
};

//////////////////////////////////////////// TEMPLATES /////////////////////////////////////////

namespace _ { // private section
// using namespace hub::sensor;

template <Size_t N = 1, Size_t... Ns>
    requires( N > 0 && ( ( Ns > 1 ) && ... ) )
class DimsT
{
  public:
    static constexpr auto size = 1 + sizeof...( Ns );
    static constexpr auto capacity() {
        Size_t ret = N;
        if constexpr ( sizeof...( Ns ) > 0 ) {
            for ( auto n : { Ns... } ) {
                ret *= n;
            }
        }
        return ret;
    }

    static constexpr auto getDim( int iDim ) {
        auto i = 0;
        for ( auto dim : { N, Ns... } ) {
            if ( i == iDim ) return dim;
            ++i;
        }
        return (Size_t)0;
    }

    static constexpr auto to_string() {
        std::string str = "";
        Size_t i        = 0;
        for ( auto dim : { N, Ns... } ) {
            str += std::to_string( dim );
            if ( i != size - 1 ) str += "x";
            ++i;
        }
        return str;
    }

    static constexpr auto getNDim() { return NDim { N, Ns... }; }
};

// template <class... Types>
//     requires( sizeof...( Types ) > 0 )
// class Type
//{
//   public:
//     static constexpr auto name() {
//         std::string str;
//         //        if constexpr ( requires { Type::name(); } ) { str += Type::name(); }
//         //        else { str += TYPE_NAME( Type ); }
//         str = ( TYPE_NAME( Types ) + ... );
//         return str;
//     }
//     static constexpr auto interpolable() { return false; }
//     static constexpr auto nByte() { return ( sizeof( Types ) + ... ); }
// };

template <class DimsT, class TypeT>
class ResolutionTI
{
  public:
    template <class Type>
        requires(std::is_same_v<TypeT, Type>)
    using get = ResolutionTI;

    template <class Type_>
    static constexpr bool has() {
        return std::is_same_v<TypeT, Type_>;
    }

//    template <class Type_>
//    static constexpr bool has() {
//        return std::is_same_v<TypeT, Type_>;
//    }

    //    using getFormatT = TypeT;
    //    static constexpr Size_t depth() {
    //        if constexpr ( requires { TypeT::depth(); } ) { return TypeT::depth() + 1; }
    //        else { return 0; }
    //    }

//    template <class Output>
//    static void write( Output& output ) {
//        output.write( getResolution() );
//    }

    static struct {
    } notReadable;

    static constexpr auto nByte() {
        auto size = 0;
        if constexpr ( requires { TypeT::nByte(); } ) { size = TypeT::nByte(); }
        else { size = sizeof( TypeT ); }
        size *= DimsT::capacity();
        return size;
    }
    static_assert( nByte() > 0 );

    static constexpr auto nDim() { return DimsT::size; };
    static constexpr auto getDim( int iDim ) { return DimsT::getDim( iDim ); }

    static constexpr std::string name() {
        //        std::string str = "<";
        std::string str;
        if constexpr ( requires { TypeT::name(); } ) { str += TypeT::name(); }
        else { str += TYPE_NAME( TypeT ); }

        if ( !( nDim() == 1 && getDim( 0 ) == 1 ) ) { str += ":" + DimsT::to_string(); }
        //        str += ">";
        return str;
    }

//    static constexpr auto has() {
//        return std::is_same_v<TypeT, Type_>();
//        return true;
//    }

    static constexpr auto nResolution = 1;

    template <class DimsT_, class FormatT_>
    constexpr bool operator==( const ResolutionTI<DimsT_, FormatT_>& ) const {
        return std::is_same_v<DimsT, DimsT_> && std::is_same_v<TypeT, FormatT_>;
    }

    template <class DimsT_, class FormatT_>
    SRC_API friend std::ostream& operator<<( std::ostream& os,
                                             const ResolutionTI<DimsT_, FormatT_>& resolution );

//    constexpr auto operator==( const Resolution& other ) const { return getResolution() == other; }
//    static constexpr Resolution getResolution() {
//        return Resolution { getFormat(), DimsT::getNDim() };
//    }

  private:
    static constexpr Format getFormat() {
        if constexpr ( requires { TypeT::name(); } ) {
            return Format { sizeof( TypeT ), TypeT::name(), TypeT::interpolable() };
        }
        else { return Format { sizeof( TypeT ), TYPE_NAME( TypeT ), true }; }
    }
};

template <class DimsT_, class FormatT_>
SRC_API std::ostream& operator<<( std::ostream& os,
                                  const ResolutionTI<DimsT_, FormatT_>& resolution ) {
    os << resolution.name();
    return os;
}

} // namespace _

// template <Size_t N, class... Formats>
// using Resolution1DT = _::ResolutionTI<_::DimsT<N>, _::TypeT<Formats...>>;
//  template <Size_t N, class... Formats>
//  using Res1DT = _::ResolutionTI<Dims<N>, TypeT<Formats...>>;

// template <Size_t N, Size_t N2, class... Formats>
// using Resolution2DT = _::ResolutionTI<_::DimsT<N, N2>, _::TypeT<Formats...>>;
//  template <Size_t N, Size_t N2, class... Formats>
//  using Res2DT = _::ResolutionTI<Dims<N, N2>, TypeT<Formats...>>;

// template <class Type, Size_t... NDimTs>
// static_assert(sizeof(ResolutionTI<Type, NDimTs...>()) == 0);
// static_assert(sizeof(Reso))

template <class Type, Size_t... Dims>
//    requires( N >= 1 && ( ( Dims > 1 ) && ... ) )
//    requires(  ( Dims > 1 ) && ...  )
// typename std::enable_if<(Dims > 0) ...>, class>
class ResolutionT : public _::ResolutionTI<_::DimsT<Dims...>, Type>
{
  public:

    template <int i>
    static constexpr Size_t n() {
        static_assert( 0 <= i && i < _::ResolutionTI<_::DimsT<Dims...>, Type>::nDim() );
        int j = 0;
        for ( const auto& n : { Dims... } ) {
            if ( i == j ) return n;
            ++j;
        }
        return 0;
    }
};

template <class Type, Size_t N>
    requires( N > 1 )
class ResolutionT<Type, N> : public _::ResolutionTI<_::DimsT<N>, Type>
{
  public:
    static constexpr auto length() { return N; }
};

template <class Type, Size_t N, Size_t N2>
class ResolutionT<Type, N, N2> : public _::ResolutionTI<_::DimsT<N, N2>, Type>
{
  public:
//    using _::ResolutionTI<_::DimsT<N, N2>, Type>::has;

    //    using type = ResolutionT<Type, N, N2>;
    static constexpr Size_t width() { return N; }
    static constexpr Size_t height() { return N2; }
};

template <class Type, Size_t N, Size_t N2, Size_t N3>
class ResolutionT<Type, N, N2, N3> : public _::ResolutionTI<_::DimsT<N, N2, N3>, Type>
{
  public:
    static constexpr Size_t nx() { return N; }
    static constexpr Size_t ny() { return N2; }
    static constexpr Size_t nz() { return N3; }
};

template <class Type, Size_t N, Size_t N2, Size_t N3, Size_t N4>
class ResolutionT<Type, N, N2, N3, N4> : public _::ResolutionTI<_::DimsT<N, N2, N3, N4>, Type>
{
  public:
    static constexpr Size_t nx() { return N; }
    static constexpr Size_t ny() { return N2; }
    static constexpr Size_t nz() { return N3; }
    static constexpr Size_t nt() { return N4; }
};

// template <class Type, Size_t... Dims>
// constexpr auto Resolution() {
//     return ResolutionT<Type, Dims...>();
// }

namespace resolution {

using Full_HD      = ResolutionT<format::RGB8, 1920, 1080>;
using Full_HD_Plus = ResolutionT<format::RGB8, 2160, 1080>;
using UltraWide    = ResolutionT<format::RGB8, 2560, 1080>;

} // namespace resolution
} // namespace sensor
} // namespace hub

// namespace hub {
// }
