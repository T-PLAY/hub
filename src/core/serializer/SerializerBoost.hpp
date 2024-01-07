
#pragma once

// #ifdef HUB_USE_ZPP_BITS

#include <array>
// #include <concepts>

// #    include "zpp_bits.h"
// #include "serializer.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

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

class SerializerBoost : public SerializerI
{
  public:
    SerializerBoost( ByteView& byteView ) : SerializerI( byteView ) {
        // m_buff.clear();
        // m_in.reset(0);
        // m_out.reset(0);
    }

    void resetIn() override {
        // m_buff.clear();
        assert( !m_buff.empty() );
        // m_in.reset(m_buff.size());
    }
    void resetOut() override {
        // m_buff.clear();
        // m_out.reset(m_buff.size());
    }
    Size_t inPosition() const override {
        // return m_in.offset();
        return 0;
    }
    Size_t outPosition() const override {
        // return m_out.offset();
        return 0;
    }
    void setInPosition( Size_t inPosition ) override {
        // m_in.reset( inPosition );
    }
    void setOutPosition( Size_t outPosition ) override {
        // m_out.reset( outPosition );
    }

    template <class... Ts>
    void serialize( const Ts&... ts ) {
        assert( false );
        // m_out(ts...);
    }

    template <class... Ts>
    void deserialize( const Ts&... ts ) {
        assert( false );
    }

  private:
    // zpp::serializer::memory_output_archive m_out { m_buff };
    // zpp::serializer::memory_input_archive m_in { m_buff };
    // Size_t m_inPosition  = 0;
    // Size_t m_outPosition = 0;
};

} // namespace serializer
} // namespace hub

// #endif
