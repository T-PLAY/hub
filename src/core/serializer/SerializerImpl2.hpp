
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

    void reset() override {
    }
    Size_t position() const override {
        return 0;
    }
    void setPosition(Size_t newPosition) override {
    }

    template <class... Ts>
    void serialize(const Ts&... ts) {
    }

    template <class... Ts>
    void deserialize(const Ts&... ts) {
    }
};

} // namespace serializer
} // namespace hub

// #endif
