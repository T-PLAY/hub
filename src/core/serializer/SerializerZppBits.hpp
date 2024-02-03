/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/10/31

#pragma once

#ifdef HUB_USE_ZPP_BITS

#    include <array>

#    include "zpp_bits.h"

#    include "SerializerI.hpp"
#    include "core/InputBase.hpp"
#    include "core/OutputBase.hpp"

namespace hub {
namespace serializer {

class SerializerZppBits : public SerializerI
{
  public:
    SerializerZppBits( ByteView& byteView ) : SerializerI( byteView ) {
        m_in.reset( 0 );
        m_out.reset( 0 );
    }

    void resetIn() override {
        assert( !m_buff.empty() );
        m_in.reset( 0 );
    }
    void resetOut() override {
        m_buff.clear();
        m_out.reset( 0 );
    }
    Size_t inPosition() const override { return m_in.position(); }
    Size_t outPosition() const override { return m_out.position(); }
    void setInPosition( Size_t inPosition ) override { m_in.reset( inPosition ); }
    void setOutPosition( Size_t outPosition ) override { m_out.position() = outPosition; }

    template <class... Ts>
    void serialize( const Ts&... ts ) {

        m_out( ts... );
    }

    template <class... Ts>
    void deserialize( Ts&... ts ) {

        m_in( ts... );
    }

  private:
    zpp::bits::out<ByteView> m_out { m_buff };
    zpp::bits::in<ByteView> m_in { m_buff };

    // Size_t m_inPosition = 0;
    // Size_t m_outPosition = 0;
};

} // namespace serializer
} // namespace hub

#endif
