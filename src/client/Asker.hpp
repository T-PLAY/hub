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

#include "asker/AskerServer2.hpp"

namespace hub {
namespace client {

///
/// \brief Asker
///
using Asker = AskerServer2;

static_assert( std::is_base_of<client::AskerInterface, Asker>::value,
               "AskerInterface is base class of Asker" );

} // namespace client
} // namespace hub
