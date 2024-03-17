/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright © 2021-2024 Hub. All Rights Reserved.
 * @author Gauthier Bouyjou <gauthierbouyjou@aol.com>
 * @date 2023/11/03
 */

#pragma once

#include "ClientSocketI.hpp"
#include "core/Macros.hpp"

namespace hub {
namespace net {

///
/// \brief The ServerSocketI class
///
class SRC_API ServerSocketI
{
  public:
    ///
    /// \brief getPort
    /// \return
    ///
    virtual int getPort() const = 0;

    ///
    /// \brief waitNewClient
    /// \return
    ///
    ClientSocketI waitNewClient();
};

} // namespace net
} // namespace hub
