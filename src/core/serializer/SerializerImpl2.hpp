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
    ///
    /// \brief SerializerImpl2
    /// \param byteView
    ///
    SerializerImpl2( ByteView& byteView ) : SerializerI( byteView ) {}

    ///
    /// \brief resetIn
    ///
    void resetIn() override {}

    ///
    /// \brief resetOut
    ///
    void resetOut() override {}

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
        assert( false );
    }

    ///
    /// \brief deserialize
    /// \param ts
    ///
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
