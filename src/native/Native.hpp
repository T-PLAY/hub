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

// namespace hub {
// /// @brief The native namespace
// /// Export these "C" functions usable by Unity C++ plugin or Matlab (loadlibrary).
// ///
// namespace native {}
// } // namespace hub

#include "core/Configuration.hpp"

#include "native_Any.hpp"
#include "native_Header.hpp"
#include "native_MetaData.hpp"
#include "native_OutputStream.hpp"
#include "native_Viewer.hpp"
#include "native_ViewerHandler.hpp"

#ifndef HUB_NON_BUILD_SENSOR
#include "sensor/native_Acquisition.hpp"
#    include "sensor/native_InputSensor.hpp"
#    include "sensor/native_OutputSensor.hpp"
#    include "sensor/native_SensorSpec.hpp"
#endif
