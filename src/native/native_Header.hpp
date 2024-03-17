/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright © 2021-2024 Hub. All Rights Reserved.
 * @author Gauthier Bouyjou <gauthierbouyjou@aol.com>
 * @date 2023/12/15
 */

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
