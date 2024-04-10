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
*   - Gauthier Bouyjou (IRIT, Toulouse Tech Transfer)
*******************************************************************************/

#pragma once

#include <vector>
// #include <stdio.h>
#include <cstdint>
//#include <iostream>

namespace hub {

///
/// \brief Data_t
///
using Data_t  = unsigned char;

///
/// \brief Datas_t
///
using Datas_t = std::vector<Data_t>;

///
/// \brief Size_t
///
using Size_t  = uint64_t; // max = 18'446'744'073'709'551'616 ~= 18 exa

}
