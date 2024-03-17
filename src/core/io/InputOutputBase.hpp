/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright © 2021-2024 Hub. All Rights Reserved.
 * @author Gauthier Bouyjou <gauthierbouyjou@aol.com>
 * @date 2023/11/09
 */

#pragma once

#include "core/InputBase.hpp"
#include "core/OutputBase.hpp"

namespace hub {
namespace io {

///
/// \brief The InputOutputBase class
///
class InputOutputBase : public InputBase, public OutputBase
{
  public:
};

} // namespace io
} // namespace hub
