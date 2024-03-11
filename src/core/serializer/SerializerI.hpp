/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/10/31

#pragma once

#include "core/Macros.hpp"

namespace hub {
namespace serializer {

///
/// \brief The SerializerI class
///
class SRC_API SerializerI
{
  public:
    using ByteView = std::vector<Data_t>;
    SerializerI( ByteView& byteView ) : m_buff { byteView } {};

    virtual Size_t inPosition() const  = 0;
    virtual Size_t outPosition() const = 0;

    virtual void resetIn()  = 0;
    virtual void resetOut() = 0;

    const ByteView& getBuff() const { return m_buff; }

  protected:
    ByteView& m_buff;
};

} // namespace serializer
} // namespace hub
