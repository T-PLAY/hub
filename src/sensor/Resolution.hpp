
#pragma once

// #include <algorithm>
#include <type_traits>

#include "Format.hpp"
#include "core/Macros.hpp"
// #include "resolution/NDim.hpp"
// #include "Resolutions.hpp"

namespace hub {
namespace sensor {

// namespace resolution {
// }

class Resolution
{
  public:
    using NDim = std::vector<Size_t>;

    Format format;
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

//} // namespace sensor
//} // namespace hub

namespace priv { // private section
// using namespace hub::sensor;

template <Size_t... Ns>
    requires( ( Ns > 0 ) && ... )
class Dims
{
  public:
    //    static struct {
    //    } isDim;
    static constexpr auto Size     = sizeof...( Ns );
    static constexpr auto Capacity = ( Ns * ... );

    static constexpr auto getDim( int iDim ) {
        auto i = 0;
        for ( auto dim : { Ns... } ) {
            if ( i == iDim ) return dim;
            ++i;
        }
        return (Size_t)0;
    }

    static constexpr auto to_string() {
        std::string str = "";
        Size_t i        = 0;
        for ( auto dim : { Ns... } ) {
            str += std::to_string( dim );
            if ( i != Size - 1 ) str += "x";
            ++i;
        }
        return str;
    }

    static constexpr auto getNDim() { return Resolution::NDim { Ns... }; }
    //    typedef int value_type;
    //    static constexpr size_t size() noexcept { return sizeof...( Ns ); }
};

template <class... Types>
    requires( sizeof...( Types ) > 0 )
class FormatT
{
  public:
    static constexpr auto name() {
        std::string str;
        //        if constexpr ( requires { FormatT::name(); } ) { str += FormatT::name(); }
        //        else { str += TYPE_NAME( FormatT ); }
        str = ( TYPE_NAME( Types ) + ... );
        return str;
    }
    static constexpr auto interpolable() { return false; }
    static constexpr auto nByte() { return ( sizeof( Types ) + ... ); }
};

// template <class FormatT, Size_t... NDimTs>
template <class DimsT, class FormatT>
//    requires( ( NDimTs > 0 ) && ... )
class ResolutionTI
{
  public:
    //    using typeFormat = FormatT;
    using getFormatT = FormatT;

    //    static constexpr Resolution resolution = getResolution();

    template <class Output>
    static void write( Output& output ) {
        //        output.write( Resolution { getFormat(), getNDim() } );
        output.write( getResolution() );
    }

    static struct {
    } notReadable;
    //    using notReadable = typename std::true_type;

    static constexpr auto nByte() {
        //        auto size = sizeof( FormatT );
        auto size = 0;
        //        if constexpr (! std::is_arithmetic_v<FormatT>) {
        if constexpr ( requires { FormatT::nByte(); } ) {
            //        if constexpr (std::is_class_v<FormatT>) {
            //        if constexpr (sizeof(FormatT) > 0) {
            size = FormatT::nByte();
        }
        else { size = sizeof( FormatT ); }
        size *= DimsT::Capacity;
        //        auto size = (std::is_class_v<FormatT>) ?( FormatT::nByte()) :(sizeof(FormatT));
        //        for ( auto dim : { NDimTs... } ) {
        //            size *= dim;
        //        }
        return size;
    }
    static_assert( nByte() > 0 );

    static constexpr auto nDim() { return DimsT::Size; };
    //    static constexpr auto nDim() { return sizeof...( NDimTs ); }
    static constexpr auto getDim( int iDim ) { return DimsT::getDim( iDim ); }
    static constexpr std::string typeName() {
        std::string str = "<";
        if constexpr ( requires { FormatT::name(); } ) { str += FormatT::name(); }
        else { str += TYPE_NAME( FormatT ); }

        if ( !( nDim() == 1 && getDim( 0 ) == 1 ) ) {

            str += ":";

            str += DimsT::to_string();

            //            for ( auto dim : { NDimTs... } ) {
            //                str += std::to_string( dim );
            //                if ( i != nDim() - 1 ) str += "x";
            //                ++i;
            //            }
        }
        //            int _[]  = { ( str += std::to_string(NDimTs) + ", " )... };
        str += ">";
        return str;
    }
    static constexpr auto nResolution = 1;
    //    static constexpr Size_t nResolution() { return 1; }

    //    template <class Format_, Size_t... Dims_>
    //    constexpr bool operator==( const ResolutionTI<Format_, Dims_...>& resolution ) const {
    template <class DimsT_, class FormatT_>
    constexpr bool operator==( const ResolutionTI<DimsT_, FormatT_>& ) const {
        return std::is_same_v<DimsT, DimsT_> && std::is_same_v<FormatT, FormatT_>;
        //            if ( std::is_same_v<FormatT, Format_> && nDim() == resolution.nDim() ) {
        //                int i = 0;
        //                for ( auto dim : { NDimTs... } ) {
        //                    if ( dim != resolution.getDim( i ) ) return false;
        //                    ++i;
        //                }
        //                return true;
        //            }
        //            else { return false; }
    }

    //        template <class Format_, Size_t... DimsT>
    template <class DimsT_, class FormatT_>
    SRC_API friend std::ostream& operator<<( std::ostream& os,
                                             const ResolutionTI<DimsT_, FormatT_>& resolution );

    constexpr auto operator==( const Resolution& other ) const {
        return getResolution() == other;
        //        Resolution me { getFormat(), getNDim() };
        //        return me == other;
    }
    static constexpr Resolution getResolution() {
        return Resolution { getFormat(), DimsT::getNDim() };
    }

  private:
    static constexpr Format getFormat() {
        if constexpr ( requires { FormatT::name(); } ) {
            return Format { sizeof( FormatT ), FormatT::name(), FormatT::interpolable() };
        }
        else { return Format { sizeof( FormatT ), TYPE_NAME( FormatT ), true }; }
    }
    //        static constexpr auto getNDim() { return Resolution::NDim { NDimTs... }; }
};

// inline template<class Id>
// template <class FormatT, Size_t... NDimTs>
//     constexpr auto ResolutionTI<FormatT, NDimTs...>::getResolutionT(int id) {
//     static_assert(id == 0);
//     return ResolutionTI<FormatT, NDimTs...>();
//     //        return
//     //        auto i = 0;
//     //        for ( auto dim : { NDimTs... } ) {
//     //            if ( i == iDim ) return dim;
//     //            ++i;
//     //        }
//     //        return (Size_t)0;
// }

// template <class FormatT, Size_t... NDimTs>
// std::ostream& operator<<( std::ostream& os, const ResolutionTI<FormatT, NDimTs...>& resolution )
template <class DimsT_, class FormatT_>
SRC_API std::ostream& operator<<( std::ostream& os,
                                  const ResolutionTI<DimsT_, FormatT_>& resolution ) {
    os << resolution.typeName();
    return os;
}

} // namespace priv
// using namespace Private;

// namespace hub {
// namespace sensor {

template <Size_t N, class... Formats>
using Resolution1DT = priv::ResolutionTI<priv::Dims<N>, priv::FormatT<Formats...>>;
// template <Size_t N, class... Formats>
// using Res1DT = priv::ResolutionTI<Dims<N>, FormatT<Formats...>>;

template <Size_t N, Size_t N2, class... Formats>
using Resolution2DT = priv::ResolutionTI<priv::Dims<N, N2>, priv::FormatT<Formats...>>;
// template <Size_t N, Size_t N2, class... Formats>
// using Res2DT = priv::ResolutionTI<Dims<N, N2>, FormatT<Formats...>>;

// template <class FormatT, Size_t... NDimTs>
// static_assert(sizeof(ResolutionTI<FormatT, NDimTs...>()) == 0);
// static_assert(sizeof(Reso))

template <class FormatT, Size_t N = 1, Size_t... NDimTs>
    requires( N >= 1 && ( ( NDimTs > 1 ) && ... ) )
// typename std::enable_if<(NDimTs > 0) ...>, class>
class ResolutionT : public priv::ResolutionTI<priv::Dims<N, NDimTs...>, FormatT>
{
  public:
    template <int i>
    static constexpr Size_t n() {
        static_assert( 0 <= i && i < priv::ResolutionTI<priv::Dims<N, NDimTs...>, FormatT>::nDim() );
        int j = 0;
        for ( const auto& n : { N, NDimTs... } ) {
            if ( i == j ) return n;
            ++j;
        }
        return 0;
    }
};
// using ResT = ResolutionT;

template <class FormatT, Size_t... DimTs>
using ResT = ResolutionT<FormatT, DimTs...>;

template <class FormatT, Size_t N>
    requires( N > 1 )
class ResolutionT<FormatT, N> : public priv::ResolutionTI<priv::Dims<N>, FormatT>
{
  public:
    static constexpr auto length() { return N; }
};

template <class FormatT, Size_t N, Size_t N2>
class ResolutionT<FormatT, N, N2> : public priv::ResolutionTI<priv::Dims<N, N2>, FormatT>
{
  public:
    static constexpr Size_t width() { return N; }
    static constexpr Size_t height() { return N2; }
};

template <class FormatT, Size_t N, Size_t N2, Size_t N3>
class ResolutionT<FormatT, N, N2, N3> : public priv::ResolutionTI<priv::Dims<N, N2, N3>, FormatT>
{
  public:
    static constexpr Size_t nx() { return N; }
    static constexpr Size_t ny() { return N2; }
    static constexpr Size_t nz() { return N3; }
};

template <class FormatT, Size_t N, Size_t N2, Size_t N3, Size_t N4>
class ResolutionT<FormatT, N, N2, N3, N4> : public priv::ResolutionTI<priv::Dims<N, N2, N3, N4>, FormatT>
{
  public:
    static constexpr Size_t nx() { return N; }
    static constexpr Size_t ny() { return N2; }
    static constexpr Size_t nz() { return N3; }
    static constexpr Size_t nt() { return N4; }
};

namespace resolution {

using Full_HD      = ResolutionT<format::RGB8, 1920, 1080>;
using Full_HD_Plus = ResolutionT<format::RGB8, 2160, 1080>;
using UltraWide    = ResolutionT<format::RGB8, 2560, 1080>;

} // namespace resolution
} // namespace sensor
} // namespace hub
