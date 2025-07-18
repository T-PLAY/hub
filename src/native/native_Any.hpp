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
*   - Nicolas Mellado (IRIT)
*   - Gauthier Bouyjou (IRIT)
*******************************************************************************/

#pragma once

#include "core/Base.hpp"

namespace hub {

class Any;

namespace native {

#ifdef __cplusplus
extern "C"
{
#endif

    ///
    /// \brief Get double from any ptr
    /// \param any [in] to get double
    /// \return double of any
    ///
    SRC_API double any_getDouble( const Any* any );

    ///
    /// \brief Get int from any ptr
    /// \param any [in] to get int
    /// \return int of any
    ///
    SRC_API int any_getInt( const Any* any );

#ifdef __cplusplus
} // end extern "C"
#endif

} // namespace native
} // namespace hub
