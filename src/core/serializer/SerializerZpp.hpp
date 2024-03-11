/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2024/01/07

#pragma once

#include <array>

#include "zpp/serializer.h"

#include "SerializerI.hpp"
#include "core/InputBase.hpp"
#include "core/OutputBase.hpp"

namespace hub {
namespace serializer {

///
/// \brief The SerializerZpp class
///
class SRC_API SerializerZpp : public SerializerI
{
  public:
    ///
    /// \brief SerializerZpp
    /// \param byteView
    ///
    SerializerZpp( ByteView& byteView ) : SerializerI( byteView ) {
        m_buff.clear();
        m_in.reset( 0 );
        m_out.reset( 0 );
    }

    ///
    /// \brief resetIn
    ///
    void resetIn() override {
        assert( !m_buff.empty() );

        m_inPosition = 0;
    }

    ///
    /// \brief resetOut
    ///
    void resetOut() override {
        m_buff.clear();
        m_outPosition = 0;
    }

    ///
    /// \brief inPosition
    /// \return
    ///
    Size_t inPosition() const override { return m_inPosition; }

    ///
    /// \brief outPosition
    /// \return
    ///
    Size_t outPosition() const override { return m_outPosition; }

    ///
    /// \brief serialize
    /// \param ts
    ///
    template <class... Ts>
    void serialize( const Ts&... ts ) {
        const auto lastBuffSize = m_buff.size();

        m_out( ts... );

        const auto curBuffSize = m_buff.size();
        assert( curBuffSize > lastBuffSize );
        m_outPosition += curBuffSize - lastBuffSize;
    }

    ///
    /// \brief deserialize
    /// \param ts
    ///
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

    Size_t m_inPosition  = 0;
    Size_t m_outPosition = 0;
};

} // namespace serializer
} // namespace hub
