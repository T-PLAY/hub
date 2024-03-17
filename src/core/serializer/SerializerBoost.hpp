/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright © 2021-2024 Hub. All Rights Reserved.
 * @author Gauthier Bouyjou <gauthierbouyjou@aol.com>
 * @date 2024/01/07
 */

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
