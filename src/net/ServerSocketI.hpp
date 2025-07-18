/*******************************************************************************
* Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
* Please visit https://www.irit.fr/tplay/.
*
* All rights reserved.
* This code belongs to tplay/hub project (https://github.com/T-PLAY/hub).
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*
* Initial Contributors:
*   - Gauthier Bouyjou
*******************************************************************************/

#pragma once

#include "core/Base.hpp"

namespace hub {
namespace net {

///
/// \brief The ServerSocketI class
/// is an interface describing server socket basic features.
///
class SRC_API ServerSocketI
{
  public:
    ///
    /// \brief Getter function
    /// \return port
    ///
    virtual int getPort() const = 0;

};

} // namespace net
} // namespace hub
