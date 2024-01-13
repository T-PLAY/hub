/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/10/31
	
#pragma once

// #include "io/input/InputBase.hpp"

// #include "io/input/InputImpl.hpp"
// #include "io/input/InputZppBits.hpp"

#include "core/Macros.hpp"
// #include "core/InputBase.hpp"
// #include "core/OutputBase.hpp"

namespace hub {
namespace serializer {

class SRC_API SerializerI
{
  public:
    using ByteView = std::vector<Data_t>;
    SerializerI( ByteView& byteView ) : m_buff { byteView } {};

    // SerializerI(SerializerI&&) = delete;
    // SerializerI(const SerializerI&) = delete;

    virtual Size_t inPosition() const                = 0;
    virtual Size_t outPosition() const                = 0;

    // virtual void setInPosition( Size_t inPosition ) = 0;
    // virtual void setOutPosition( Size_t outPosition ) = 0;

    virtual void resetIn()                           = 0;
    virtual void resetOut()                           = 0;

    const ByteView& getBuff() const {
        return m_buff;
    }

  // private:
  protected:
    ByteView& m_buff;
};

} // namespace serializer
} // namespace hub
