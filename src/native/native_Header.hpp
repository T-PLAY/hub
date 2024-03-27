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

#ifndef HUB_CPP_SOURCE
namespace io {
class Header;
}
#endif

namespace native {

#ifdef __cplusplus
extern "C"
{
#endif

    ///
    /// \brief createHeader
    /// \param dataSize
    /// \return
    ///
    io::Header* createHeader( Size_t dataSize );


    ///
    /// \brief freeHeader
    /// \param header
    ///
    SRC_API void freeHeader( io::Header* header );

#ifdef __cplusplus
} // end extern "C"
#endif

} // namespace native
} // namespace hub
