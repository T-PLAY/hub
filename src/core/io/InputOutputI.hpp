#pragma once

//#include "core/Macros.hpp"
//#include "BasicInputOutputI.hpp"
//#include "input/InputI.hpp"
//#include "output/OutputI.hpp"
#include "core/Input.hpp"
#include "core/Output.hpp"
//#include "core/Serializer.hpp"

namespace hub {
namespace io {

//class InputOutputI : public input::InputI, public output::OutputI
////class InputOutputI : public BasicInputOutputI
////class InputOutputI : public input::InputOutputI
//{
//  public:
//    using input::InputI::read;
//    using output::OutputI::write;
//};

//template <class SerializerT = Serializer, class InputT = Input<SerializerT>, class OutputT = Output<SerializerT>>
//class InputOutputI : public InputT, public OutputT
//{
//  public:
//    using InputT::read;
//    using OutputT::write;
//};

} // namespace io
} // namespace hub
