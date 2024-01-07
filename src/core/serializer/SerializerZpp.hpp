
#pragma once

// #ifdef HUB_USE_ZPP_BITS

#include <array>
// #include <concepts>

// #    include "zpp_bits.h"
#include "serializer.h"

// #include "io/input/InputBase.hpp"

// #include "io/input/InputImpl.hpp"
// #include "io/input/InputZppBits.hpp"

#include "SerializerI.hpp"
#include "core/InputBase.hpp"
#include "core/OutputBase.hpp"

namespace hub {
namespace serializer {

// template <class Out, class... Ts>
// concept Outable = requires(Out & out, Ts&... ts) {out(ts...); };

// template <class In, class... Ts>
// concept Inable = requires(In & in, Ts&... ts) {in(ts...); };

// template <class T>
// concept isMap = requires( T t ) {
//     t.begin()->first;
//     t.begin()->second;
// };

class SerializerZpp : public SerializerI
{
  public:
    SerializerZpp( ByteView& byteView ) : SerializerI( byteView ) {
        m_buff.clear();
        m_in.reset( 0 );
        m_out.reset( 0 );
    }

    void resetIn() override {
        assert( !m_buff.empty() );
        // m_in.reset( 0 );
        m_inPosition = 0;
    }
    void resetOut() override {
        m_buff.clear();
        // m_out.reset( 0 );
        m_outPosition = 0;
    }
    Size_t inPosition() const override { return m_inPosition; }
    Size_t outPosition() const override { return m_outPosition; }
    // void setInPosition( Size_t inPosition ) override {
    //     m_inPosition = inPosition;
    // }
    // void setOutPosition( Size_t outPosition ) override { m_outPosition = outPosition; }

    // template <class T>
    // void serialize( const T& t ) {
    //     const auto lastBuffSize = m_buff.size();

    //     m_out( t );

    //     const auto curBuffSize = m_buff.size();
    //     assert( curBuffSize > lastBuffSize );
    //     m_outPosition += curBuffSize - lastBuffSize;
    // }

    template <class... Ts>
    void serialize( const Ts&... ts ) {
        const auto lastBuffSize = m_buff.size();

        m_out( ts... );

        const auto curBuffSize = m_buff.size();
        assert( curBuffSize > lastBuffSize );
        m_outPosition += curBuffSize - lastBuffSize;
    }

    // template <class T>
    // void deserialize( T& t ) {
    //     const auto lastBuffSize = m_buff.size();

    //     m_in( t );

    //     const auto curBuffSize = m_buff.size();
    //     assert( curBuffSize < lastBuffSize );
    //     m_inPosition += lastBuffSize - curBuffSize;
    // }

    template <class... Ts>
    void deserialize( Ts&... ts ) {
        const auto lastBuffSize = m_buff.size();

        m_in( ts... );

        const auto curBuffSize = m_buff.size();
        assert( curBuffSize < lastBuffSize );
        m_inPosition += lastBuffSize - curBuffSize;
    }

  private:
    zpp::serializer::memory_output_archive m_out { m_buff };
    zpp::serializer::memory_input_archive m_in { m_buff };

    Size_t m_inPosition = 0;
    Size_t m_outPosition = 0;
};

} // namespace serializer
} // namespace hub

// #endif
