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

#ifndef HUB_CPP_SOURCE

// class MetaData;

namespace data {
class Mesh;
class Shape;
}
#endif

namespace native {

#ifdef __cplusplus
extern "C"
{
#endif

    SRC_API int mesh_getNShape( const data::Mesh* mesh );

    SRC_API const data::Shape * mesh_getShape( const data::Mesh* mesh, int iShape );

    //SRC_API void to_string( const sensor::SensorSpec* sensorSpec );
    ///
    /// \brief sensorSpec_toString
    /// \param sensorSpec
    /// \param output
    /// \param strLen
    ///
    SRC_API void mesh_toString( const data::Mesh* mesh, char* output, int* strLen );

    SRC_API void shape_getName( const data::Shape* shape, char* output, int* strLen );

    SRC_API void mesh_getColor( const data::Mesh* mesh, int iShape, float* colors );

    SRC_API int shape_getNVertice( const data::Shape* shape );

    SRC_API void shape_getVertices(const data::Shape * shape, float * vertices, int * size);

    SRC_API int shape_getNIndice( const data::Shape* shape );

    SRC_API void shape_getIndices(const data::Shape * shape, int * indices, int * size);


#ifdef __cplusplus
} // end extern "C"
#endif

} // namespace native
} // namespace hub
