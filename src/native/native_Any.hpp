/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright © 2021-2024 Hub. All Rights Reserved.
 * @author Gauthier Bouyjou <gauthierbouyjou@aol.com>
 * @date 2023/12/07
 */

#pragma once

#include "core/Macros.hpp"

namespace hub {

class Any;

namespace native {

#ifdef __cplusplus
extern "C"
{
#endif

    ///
    /// \brief any_getDouble
    /// \param any
    /// \return
    ///
    SRC_API double any_getDouble( const Any* any );

    ///
    /// \brief any_getInt
    /// \param any
    /// \return
    ///
    SRC_API int any_getInt( const Any* any );

#ifdef __cplusplus
} // end extern "C"
#endif

} // namespace native
} // namespace hub
