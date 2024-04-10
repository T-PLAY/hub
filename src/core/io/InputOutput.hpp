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

#include "core/Input.hpp"
#include "core/Output.hpp"
#include "core/Serializer.hpp"

namespace hub {
namespace io {

///
/// \brief The InputOutput class
///
template <class SerializerT = Serializer,
          class InputT      = InputT<SerializerT>,
          class OutputT     = OutputT<SerializerT>>
class InputOutput : public InputT, public OutputT
{
  public:
    using InputT::read;
    using OutputT::write;
};

} // namespace io
} // namespace hub
