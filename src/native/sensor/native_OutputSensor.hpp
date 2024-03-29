/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/12/13

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
