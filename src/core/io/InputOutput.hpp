/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/10/28
	
#pragma once

// #include "io/InputOutputBase.hpp"
// #include "io/input/InputBase.hpp"
// #include "io/output/OutputBase.hpp"

#include "core/Input.hpp"
#include "core/Output.hpp"
//#include "core/Serializer.hpp"
//#include "InputOutputImpl.hpp"
//#include "InputOutputZppBits.hpp"
//#include "core/io/InputOutputBase.hpp"
#include "core/Serializer.hpp"

namespace hub {
namespace io {

// using InputOutput = io::InputOutputImpl;
//using InputOutput = io::InputOutputZppBits;

//using InputOutput = io::InputOutputBase<SerializerT>;

//template <class SerializerT = Serializer>
template <class SerializerT = Serializer, class InputT = InputT<SerializerT>, class OutputT = OutputT<SerializerT>>
class InputOutput : public InputT, public OutputT {
  public:
    using InputT::read;
    using OutputT::write;
};

// static_assert(std::is_base_of<io::input::InputBase, InputOutput>::value, "InputOutput is not base
// class of IntputI"); static_assert(std::is_base_of<io::output::OutputBase, InputOutput>::value,
// "InputOutput is not base class of OutputBase");

} // namespace io
} // namespace hub
