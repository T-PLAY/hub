/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2024/01/06

#pragma once

#include <array>

#include "SerializerI.hpp"
#include "core/InputBase.hpp"
#include "core/OutputBase.hpp"

namespace hub {
namespace serializer {

///
/// \brief The SerializerImpl2 class
///
class SerializerImpl2 : public SerializerI
{
  public:
    SerializerImpl2( ByteView& byteView ) : SerializerI( byteView ) {}

    void resetIn() override {}
    void resetOut() override {}
    Size_t inPosition() const override { return m_inPosition; }
    Size_t outPosition() const override { return m_outPosition; }

    template <class... Ts>
    void serialize( const Ts&... ts ) {
        assert( false );
    }

    template <class... Ts>
    void deserialize( const Ts&... ts ) {
        assert( false );
    }

  private:
    Size_t m_inPosition  = 0;
    Size_t m_outPosition = 0;
};

} // namespace serializer
} // namespace hub
