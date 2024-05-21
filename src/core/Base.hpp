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

#include "base/Macros.hpp"


#include "base/traits/SizeOf.hpp"
#include "base/traits/TypeId.hpp"
#include "base/traits/TypeName.hpp"
#include "base/traits/ToString.hpp"
#include "base/Traits.hpp"

#include "base/Defines.hpp"

#include "Configuration.hpp" // Todo move Configuration to base folder


#if CPP_VERSION <= 14
//#include "std_any/any.h"
#else
#    include <any>
#endif
