/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/12/13
	
#pragma once

#include "core/Macros.hpp"

// #include "sensor/OutputSensor.hpp"
// #include "io/output/OutputStream.hpp"

namespace hub {

#ifndef HUB_CPP_SOURCE
namespace output {
class OutputStream;
}
namespace io {
class Header;
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

    output::OutputStream* createOutputStream(
        const io::Header * header,
        const char* streamName,
                                              int port,
        const char* ipv4);

    ///
    /// \brief mat4OutputSensorSendAcq
    /// \param outputSensor
    /// \param input
    /// \return
    ///
    // SRC_API bool mat4OutputSensorSendAcq( sensor::OutputSensor* outputSensor, const float* input
    // );

    void outputStream_write_int( output::OutputStream* outputStream, int value );

    ///
    /// \brief freeOutputSensor
    /// \param outputSensor
    ///
    SRC_API void freeOutputStream( output::OutputStream* outputStream );

#ifdef __cplusplus
} // end extern "C"
#endif

} // namespace native
} // namespace hub
