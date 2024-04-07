/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/10/31

#pragma once

#include "core/Base.hpp"

namespace hub {
namespace serializer {

///
/// \brief The SerializerI class
///
class SRC_API SerializerI
{
  public:
    ///
    /// \brief ByteView
    ///
    using ByteView = std::vector<Data_t>;

    ///
    /// \brief SerializerI
    /// \param byteView
    ///
    SerializerI( ByteView& byteView ) : m_buff { byteView } {};

    ///
    /// \brief inPosition
    /// \return
    ///
    virtual Size_t inPosition() const  = 0;

    ///
    /// \brief outPosition
    /// \return
    ///
    virtual Size_t outPosition() const = 0;

    ///
    /// \brief resetIn
    ///
    virtual void resetIn()  = 0;

    ///
    /// \brief resetOut
    ///
    virtual void resetOut() = 0;

    ///
    /// \brief getBuff
    /// \return
    ///
    const ByteView& getBuff() const { return m_buff; }

  protected:
    ///
    /// \brief m_buff
    ///
    ByteView& m_buff;
};

} // namespace serializer
} // namespace hub
