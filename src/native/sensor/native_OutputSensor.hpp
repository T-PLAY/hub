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
namespace sensor {
class OutputSensor;
}
#endif

namespace native {

#ifdef __cplusplus
extern "C"
{
#endif

    ///
    /// \brief createMat4OutputSensor
    /// \param sensorName
    /// \param ipv4
    /// \param port
    /// \return
    ///
    // SRC_API sensor::OutputSensor*

    ///
    /// \brief mat4OutputSensorSendAcq
    /// \param outputSensor
    /// \param input
    /// \return
    ///
    // SRC_API bool mat4OutputSensorSendAcq( sensor::OutputSensor* outputSensor, const float* input

    ///
    /// \brief freeOutputSensor
    /// \param outputSensor
    ///
    SRC_API void freeOutputSensor( sensor::OutputSensor* outputSensor );

#ifdef __cplusplus
} // end extern "C"
#endif

} // namespace native
} // namespace hub
