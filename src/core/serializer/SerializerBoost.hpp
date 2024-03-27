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

#ifdef HUB_USE_BOOST

#    include <array>

#    include <boost/archive/text_iarchive.hpp>
#    include <boost/archive/text_oarchive.hpp>

#    include "SerializerI.hpp"
#    include "core/InputBase.hpp"
#    include "core/OutputBase.hpp"

namespace hub {
namespace serializer {

///
/// \brief The SerializerBoost class
///
class SerializerBoost : public SerializerI
{
  public:
    SerializerBoost( ByteView& byteView ) : SerializerI( byteView ) {}

    void resetIn() override { assert( !m_buff.empty() ); }
    void resetOut() override {}
    Size_t inPosition() const override { return 0; }
    Size_t outPosition() const override { return 0; }

    template <class... Ts>
    void serialize( const Ts&... ts ) {
        assert( false );
    }

    template <class... Ts>
    void deserialize( const Ts&... ts ) {
        assert( false );
    }

  private:
    // Size_t m_inPosition  = 0;
    // Size_t m_outPosition = 0;
};

} // namespace serializer
} // namespace hub

#endif
