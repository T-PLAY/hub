/*******************************************************************************
* Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
* Please visit https://www.irit.fr/tplay/.
*
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Mozilla Public License, v. 2.0
* available at https://mozilla.org/MPL/2.0/.
*
* Initial Contributors:
*   - Gauthier Bouyjou
*******************************************************************************/

#pragma once

#include "core/InputBase.hpp"
#include "core/OutputBase.hpp"

namespace hub {
namespace io {

///
/// \brief The InputOutputBase class
///
class InputOutputBase : public InputBase, public OutputBase
{
  public:
};

} // namespace io
} // namespace hub
