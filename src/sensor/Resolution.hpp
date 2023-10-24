
#pragma once

// #include <numeric>
// #include <string>
#include <algorithm>
#include <any>
#include <sstream>
#include <vector>

#include "core/Macros.hpp"
#include "core/Serial.hpp"
#include "core/Traits.hpp"
#include "core/Vector.hpp"

#include "resolution/Dims.hpp"
#include "resolution/Format.hpp"

// #include "io/input/Input.hpp"
// #include "io/output/Output.hpp"

#include "core/Input.hpp"

namespace hub {
namespace sensor {

class Resolution
{
  public:
    resolution::Format format;
    resolution::Dims dims;

    // io
    //    static constexpr Size_t ioGetSize() { return sizeof( Measure ) - sizeof( Data_t* ); }
    //    const Data_t* ioGetData() const { return (Data_t*)&m_format; }
    //    void ioSetData( const Data_t* data, Size_t size ) { memcpy( &m_format, data, size ); }
    //    constexpr std::string ioTypeName() const { return "Measure"; }
    // end io

    Resolution() = default;

  public:
    constexpr auto nByte() const {
        //        auto size = m_format.nByte;
        auto size = format.nByte;
        for ( int i = 0; i < dims.size(); ++i ) {
            size *= dims.ns[i];
        }
        return size;
    }

    constexpr bool operator==( const Resolution& resolution ) const {
        return format == resolution.format && dims == resolution.dims;
        //        return getFormat() == resolution.getFormat() && getDims() == resolution.getDims();
    }

    SRC_API friend std::ostream& operator<<( std::ostream& os, const Resolution& resolution );
};

// template <class... Ns>
// class ResolutionT
//{
//   public:
//     const resolution::Format format;
//     ResolutionT() = delete;

//    constexpr ResolutionT( resolution::Format format, Ns&&... ns ) :
//        format { format }
//    {}

//    constexpr auto getFormat() const { return format; }

//    constexpr auto nByte() const {
//        int size = format.nByte;
//        return size;
//    }

//};

/////////////////////////////////////////////////////////////

using Resolutions = std::vector<Resolution>;

std::ostream& operator<<( std::ostream& os, const Resolutions& resolutions );
std::string toString( const Resolutions& resolutions );
constexpr Size_t nByte( const Resolutions& resolutions );

constexpr Size_t nByte( const Resolutions& resolutions ) {
    Size_t size = 0;
    for ( const auto& resolution : resolutions ) {
        size += resolution.nByte();
    }
    return size;
}

} // namespace sensor
} // namespace hub
