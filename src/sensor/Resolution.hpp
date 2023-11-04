
#pragma once

// #include <algorithm>
// #include <type_traits>

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

namespace { // private

template <class FormatT, Size_t... NDimTs>
    requires( ( NDimTs > 0 ) && ... )
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

    static constexpr auto nByte() {
        auto size = sizeof( FormatT );
        for ( auto dim : { NDimTs... } ) {
            size *= dim;
        }
        return size;
    }
    static_assert( nByte() > 0 );

    static constexpr auto nDim() { return sizeof...( NDimTs ); }
    static constexpr auto getDim( int iDim ) {
        auto i = 0;
        for ( auto dim : { NDimTs... } ) {
            if ( i == iDim ) return dim;
            ++i;
        }
        return (Size_t)0;
    }
    static constexpr std::string typeName() {
        std::string str = "<";
        if constexpr ( requires { FormatT::name(); } ) { str += FormatT::name(); }
        else { str += TYPE_NAME( FormatT ); }

        if ( !( nDim() == 1 && getDim( 0 ) == 1 ) ) {

            str += ":";
            Size_t i = 0;

            for ( auto dim : { NDimTs... } ) {
                str += std::to_string( dim );
                if ( i != nDim() - 1 ) str += "x";
                ++i;
            }
        }
        //            int _[]  = { ( str += std::to_string(NDimTs) + ", " )... };
        str += ">";
        return str;
    }
    static constexpr Size_t nResolution() { return 1; }

    template <class Format_, Size_t... Dims_>
    constexpr bool operator==( const ResolutionTI<Format_, Dims_...>& resolution ) const {
        if ( std::is_same_v<FormatT, Format_> && nDim() == resolution.nDim() ) {
            int i = 0;
            for ( auto dim : { NDimTs... } ) {
                if ( dim != resolution.getDim( i ) ) return false;
                ++i;
            }
            return true;
        }
        else { return false; }
    }

    template <class Format_, Size_t... DimsT>
    SRC_API friend std::ostream& operator<<( std::ostream& os,
                                             const ResolutionTI<Format_, DimsT...>& measure );

    constexpr auto operator==( const Resolution& other ) const {
        return getResolution() == other;
        //        Resolution me { getFormat(), getNDim() };
        //        return me == other;
    }
    static constexpr Resolution getResolution() { return Resolution { getFormat(), getNDim() }; }

  private:
    static constexpr Format getFormat() {
        if constexpr ( requires { FormatT::name(); } ) {
            return Format { sizeof( FormatT ), FormatT::name(), FormatT::interpolable() };
        }
        else { return Format { sizeof( FormatT ), TYPE_NAME( FormatT ), true }; }
    }
    static constexpr auto getNDim() { return Resolution::NDim { NDimTs... }; }
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

template <class FormatT, Size_t... NDimTs>
std::ostream& operator<<( std::ostream& os, const ResolutionTI<FormatT, NDimTs...>& resolution ) {
    os << resolution.typeName();
    return os;
}

} // namespace private

// template <class FormatT, Size_t... NDimTs>
// static_assert(sizeof(ResolutionTI<FormatT, NDimTs...>()) == 0);
// static_assert(sizeof(Reso))

template <class FormatT, Size_t N = 1, Size_t... NDimTs>
    requires( ( NDimTs > 1 ) && ... )
// typename std::enable_if<(NDimTs > 0) ...>, class>
class ResolutionT : public ResolutionTI<FormatT, N, NDimTs...>
{
  public:
    template <int i>
    static constexpr Size_t n() {
        static_assert( 0 <= i && i < ResolutionTI<FormatT, N, NDimTs...>::nDim() );
        int j = 0;
        for ( const auto& n : { N, NDimTs... } ) {
            if ( i == j ) return n;
            ++j;
        }
        return 0;
    }
};

template <class FormatT, Size_t N>
class ResolutionT<FormatT, N> : public ResolutionTI<FormatT, N>
{
  public:
    static constexpr auto length() { return N; }
};

template <class FormatT, Size_t N, Size_t N2>
class ResolutionT<FormatT, N, N2> : public ResolutionTI<FormatT, N, N2>
{
  public:
    static constexpr Size_t width() { return N; }
    static constexpr Size_t height() { return N2; }
};

template <class FormatT, Size_t N, Size_t N2, Size_t N3>
class ResolutionT<FormatT, N, N2, N3> : public ResolutionTI<FormatT, N, N2, N3>
{
  public:
    static constexpr Size_t nx() { return N; }
    static constexpr Size_t ny() { return N2; }
    static constexpr Size_t nz() { return N3; }
};

template <class FormatT, Size_t N, Size_t N2, Size_t N3, Size_t N4>
class ResolutionT<FormatT, N, N2, N3, N4> : public ResolutionTI<FormatT, N, N2, N3, N4>
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
