/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/09/29

#pragma once

// namespace hub {
// /// @brief The native namespace
// /// Export these "C" functions usable by Unity C++ plugin or Matlab (loadlibrary).
// ///
// namespace native {}
// } // namespace hub

#include "native_Any.hpp"
#include "native_MetaData.hpp"
#include "native_OutputStream.hpp"
#include "native_Viewer.hpp"
#include "native_ViewerHandler.hpp"

#ifndef HUB_NON_BUILD_SENSOR
#    include "sensor/native_InputSensor.hpp"
#    include "sensor/native_OutputSensor.hpp"
#    include "sensor/native_SensorSpec.hpp"
#endif
