#pragma once

// #include "io/InputOutputI.hpp"
// #include "io/input/InputI.hpp"
// #include "io/output/OutputI.hpp"

#include "core/Input.hpp"
#include "core/Output.hpp"
//#include "core/Serializer.hpp"
//#include "InputOutputImpl.hpp"
//#include "InputOutputZppBits.hpp"
//#include "core/io/InputOutputI.hpp"
#include "core/Serializer.hpp"

namespace hub {
namespace io {

// using InputOutput = io::InputOutputImpl;
//using InputOutput = io::InputOutputZppBits;

//using InputOutput = io::InputOutputI<SerializerT>;

//template <class SerializerT = Serializer>
template <class SerializerT = Serializer, class InputT = InputT<SerializerT>, class OutputT = OutputT<SerializerT>>
class InputOutput : public InputT, public OutputT {
  public:
    using InputT::read;
    using OutputT::write;
};

// static_assert(std::is_base_of<io::input::InputI, InputOutput>::value, "InputOutput is not base
// class of IntputI"); static_assert(std::is_base_of<io::output::OutputI, InputOutput>::value,
// "InputOutput is not base class of OutputI");

} // namespace io
} // namespace hub
