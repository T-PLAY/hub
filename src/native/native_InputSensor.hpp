#pragma once

#include "core/Macros.hpp"

#include "sensor/InputSensor.hpp"

namespace hub {

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
    /// \brief createInputSensor
    /// \param streamName
    /// \param ipv4
    /// \param port
    /// \return
    ///
    SRC_API sensor::InputSensor* createInputSensor( const char* streamName,
         const char* ipv4 = input::InputStream::s_defaultIpv4.c_str(),
                                            int port = input::InputStream::s_defaultPort);

    ///
    /// \brief freeInputSensor
    /// \param inputSensor
    ///
    SRC_API void freeInputSensor( sensor::InputSensor* inputSensor );

    ///
    /// \brief getAcquisitionSize
    /// \param inputSensor
    /// \return
    ///
    SRC_API int getAcquisitionSize( const sensor::InputSensor* inputSensor );

    ///
    /// \brief getAcquisition
    /// \param inputSensor
    /// \return
    ///
    SRC_API sensor::Acquisition* getAcquisition( sensor::InputSensor* inputSensor );


#ifdef __cplusplus
} // end extern "C"
#endif

} // namespace native
} // namespace hub
