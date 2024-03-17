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

#ifndef HUB_CPP_SOURCE
class MetaData;
#endif

namespace native {

#ifdef __cplusplus
extern "C"
{
#endif

    ///
    /// \brief metaData_exists
    /// \param metaData
    /// \param metaName
    /// \return
    ///
    SRC_API bool metaData_exists( const MetaData* metaData, const char* metaName );

    ///
    /// \brief metaData_getString
    /// \param metaData
    /// \param metaName
    /// \param output
    /// \param strLen
    /// \return
    ///
    SRC_API bool
    metaData_getString( const MetaData* metaData, const char* metaName, char* output, int* strLen );

    ///
    /// \brief metaData_getMat4
    /// \param metaData
    /// \param metaName
    /// \param output
    /// \return
    ///
    SRC_API bool metaData_getMat4( const MetaData* metaData, const char* metaName, float* output );

    ///
    /// \brief metaData_getInt
    /// \param metaData
    /// \param metaName
    /// \return
    ///
    SRC_API int metaData_getInt( const MetaData* metaData, const char* metaName );

    ///
    /// \brief metaData_getDouble
    /// \param metaData
    /// \param metaName
    /// \return
    ///
    SRC_API double metaData_getDouble( const MetaData* metaData, const char* metaName );

#ifdef __cplusplus
} // end extern "C"
#endif

} // namespace native
} // namespace hub
