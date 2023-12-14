#pragma once

#include "core/Macros.hpp"

// #include "sensor/OutputSensor.hpp"

namespace hub {

#ifndef HUB_CPP_SOURCE
namespace sensor {
class OutputSensor;
}
#endif

///
/// @brief native
/// Export these "C" functions usable by Unity C++ plugin or Matlab (loadlibrary).
///
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
    // createMat4OutputSensor( const char* sensorName, const char* ipv4, int port );

    ///
    /// \brief mat4OutputSensorSendAcq
    /// \param outputSensor
    /// \param input
    /// \return
    ///
    // SRC_API bool mat4OutputSensorSendAcq( sensor::OutputSensor* outputSensor, const float* input
    // );

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
