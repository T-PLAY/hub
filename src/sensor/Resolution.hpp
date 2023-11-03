
#pragma once

// #include <numeric>
// #include <string>
#include <algorithm>
// #include <any>
// #include <sstream>
// #include <vector>
#include <type_traits>

#include "core/Macros.hpp"
// #include "core/Serial.hpp"
// #include "core/Traits.hpp"
// #include "core/Vector.hpp"
// #include "core/Tuple.hpp"

// #include "resolution/NDim.hpp"
// #include "resolution/Format.hpp"

// #include "io/input/Input.hpp"
// #include "io/output/Output.hpp"

// #include "core/Input.hpp"

// #include "Format.hpp"
#include "resolution/Format.hpp"
#include "resolution/NDim.hpp"

namespace hub {
namespace sensor {

class Resolution
{
  public:
    resolution::Format format;
    resolution::NDim nDim;

    template <class Serial>
    void serialize( Serial& serial ) {
        serial(format);
//        serial( a, b, name, vints );
    }

    SRC_API friend std::ostream& operator<<( std::ostream& os, const Resolution& resolution );
};

//////////////////////////////////////////// TEMPLATES /////////////////////////////////////////

template <class FormatT, Size_t... NDimT>
    requires( ( NDimT > 0 ) && ... )
class ResolutionTI
{
  public:
    //    Resolution getResolution
    static constexpr resolution::Format getFormat() {
        if constexpr ( requires { FormatT::name(); } ) {
            return resolution::Format {
                sizeof( FormatT ), FormatT::name(), FormatT::interpolable() };
        }
        else {
//            return resolution::Format { sizeof( FormatT ), TYPE_NAME( FormatT ), false };
            return resolution::Format { sizeof( FormatT ), boost::typeindex::type_id<typeof(FormatT)>().pretty_name().c_str(), false };
        }
    }
    using typeFormat = FormatT;

    static constexpr resolution::NDim getNDim() { return resolution::NDim { NDimT... }; }

    template <class Output>
    static void write( Output& output ) {
        output.write( Resolution { getFormat(), getNDim() } );
        //        const auto & hashCode = m_any.type().hash_code();
        //        output.write(hashCode);
        //        m_anyHelper->write(output, m_any);
        ////        auto & [data, size] = m_anyHelper->serialize(m_any);
        //        output.write(data, size);
    }

    static struct {
    } notReadable;

    static constexpr auto nByte() {
        auto size = sizeof( FormatT );
        for ( auto dim : { NDimT... } ) {
            size *= dim;
        }
        return size;
    }
    static_assert( nByte() > 0 );

    static constexpr auto nDim() { return sizeof...( NDimT ); }
    static constexpr auto getDim( int iDim ) {
        auto i = 0;
        for ( auto dim : { NDimT... } ) {
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

            for ( auto dim : { NDimT... } ) {
                str += std::to_string( dim );
                if ( i != nDim() - 1 ) str += "x";
                ++i;
            }
        }
        //            int _[]  = { ( str += std::to_string(NDimT) + ", " )... };
        str += ">";
        return str;
    }
    static constexpr Size_t nResolution() { return 1; }

    template <class Format_, Size_t... Dims_>
    constexpr bool operator==( const ResolutionTI<Format_, Dims_...>& resolution ) const {
        if ( std::is_same_v<FormatT, Format_> && nDim() == resolution.nDim() ) {
            int i = 0;
            for ( auto dim : { NDimT... } ) {
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
};

template <class FormatT, Size_t N = 1, Size_t... NDimT>
    requires( ( NDimT > 1 ) && ... )
// typename std::enable_if<(NDimT > 0) ...>, class>
class ResolutionT : public ResolutionTI<FormatT, N, NDimT...>
{
  public:
    template <int i>
    static constexpr Size_t n() {
        static_assert( 0 <= i && i < ResolutionTI<FormatT, N, NDimT...>::nDim() );
        int j = 0;
        for ( const auto& n : { N, NDimT... } ) {
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

template <class FormatT, Size_t... NDimT>
std::ostream& operator<<( std::ostream& os, const ResolutionTI<FormatT, NDimT...>& resolution ) {
    os << resolution.typeName();
    return os;
}

namespace resolution {

using Full_HD      = ResolutionT<format::RGB8, 1920, 1080>;
using Full_HD_Plus = ResolutionT<format::RGB8, 2160, 1080>;
using UltraWide    = ResolutionT<format::RGB8, 2560, 1080>;

} // namespace resolution
} // namespace sensor
} // namespace hub
