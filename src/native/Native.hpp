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
