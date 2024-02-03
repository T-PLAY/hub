/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/10/28

#pragma once

#include "core/Input.hpp"
#include "core/Output.hpp"
#include "core/Serializer.hpp"

namespace hub {
namespace io {

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
