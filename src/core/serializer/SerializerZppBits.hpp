#pragma once

#include <array>

#include "zpp_bits.h"

// #include "io/input/InputI.hpp"

// #include "io/input/InputImpl.hpp"
// #include "io/input/InputZppBits.hpp"

#include "SerializerI.hpp"

namespace hub {
namespace serializer {

class SerializerZppBits : public SerializerI
{
  public:
    static constexpr Size_t BuffSize = 1'000'000;

    template <class Output, class... Ts>
    void pack( Output& output, const Ts&... ts ) {
        zpp::bits::out out( m_serialBuff );
        assert( out.position() == 0 );
        out( ts... ).or_throw();

        const Size_t size = out.position();
        assert( size < BuffSize );
        output.write( reinterpret_cast<const Data_t*>( &size ), sizeof( Size_t ) );
        output.write( m_serialBuff.data(), size );
    }

    template <class Input, class... Ts>
    void unpack( Input& input, Ts&... ts ) {
        Size_t size;
        input.read( reinterpret_cast<Data_t*>( &size ), sizeof( Size_t ) );
        assert( size < BuffSize );
        input.read( m_serialBuff.data(), size );

        zpp::bits::in in( m_serialBuff );
        in( ts... ).or_throw();
    }

    //    template <class T>
    //    const auto& pack( const T& t ) {

    //        zpp::bits::out output( m_serialBuff );
    //        output( t ).or_throw();
    //        const Size_t size = output.position();
    //        assert( size < BuffSize );
    //        //        write(size);
    //        //        write( reinterpret_cast<const Data_t*>( &size ), sizeof( Size_t ) );
    //        //        write( m_serialBuff.data(), output.position() );
    //        m_result.size = size;

    //        //        m_result.size = m_serial.size();
    //        //        return m_result;
    //        //        return Result { (const Data_t*)m_serial.data(), (Size_t)m_serial.size() };
    //        return m_result;
    //    }

  private:
    std::array<unsigned char, BuffSize> m_serialBuff;
//    Result m_result { m_serialBuff.data(), 0 };
};

} // namespace serializer
} // namespace hub
