
#pragma once

// #ifdef HUB_USE_ZPP_BITS

#include <array>
// #include <concepts>

// #    include "zpp_bits.h"

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

class SerializerImpl2 : public SerializerI
{
  public:
    SerializerImpl2( ByteView& byteView ) : SerializerI( byteView ) {}

    void resetIn() override {}
    void resetOut() override {}
    Size_t inPosition() const override { return m_inPosition; }
    Size_t outPosition() const override { return m_outPosition; }
    // void setInPosition( Size_t inPosition ) override { m_inPosition = inPosition; }
    // void setOutPosition( Size_t outPosition ) override { m_outPosition = outPosition; }

    template <class... Ts>
    void serialize( const Ts&... ts ) {
        assert(false);

    }

    // void serialize(const std::string & str) {
    //     // m_buff[m_inPosition] = str.size();
    //     // memcpy(&m_buff[m_inPosition], str.max_size(), sizeof())
    // }

    template <class... Ts>
    void deserialize( const Ts&... ts ) {
        assert(false);

    }
    // void deserialize(std::string & str) {
    //     // m_buff[m_inPosition] = str.size();
    //     // memcpy(&m_buff[m_inPosition], str.max_size(), sizeof())
    // }

  private:
    Size_t m_inPosition  = 0;
    Size_t m_outPosition = 0;
};

} // namespace serializer
} // namespace hub

// #endif
