/*******************************************************************************
* Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
* Please visit https://www.irit.fr/tplay/.
*
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Mozilla Public License, v. 2.0
* available at https://mozilla.org/MPL/2.0/.
*
* Initial Contributors:
*   - Nicolas Mellado (IRIT)
*   - Gauthier Bouyjou (IRIT)
*******************************************************************************/

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
