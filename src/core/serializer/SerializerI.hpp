/*******************************************************************************
* Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
* Please visit https://www.irit.fr/tplay/.
*
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Mozilla Public License, v. 2.0
* available at https://mozilla.org/MPL/2.0/.
*
* Initial Contributors:
*   - Gauthier Bouyjou
*******************************************************************************/

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
