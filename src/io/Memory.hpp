#pragma once

#include <stdexcept>

// #include "InputOutput.hpp"
#include "Macros.hpp"
#include <cassert>

#include "input/InputMemory.hpp"
#include "output/OutputMemory.hpp"

namespace hub {
// using namespace input;
// using namespace output;
namespace io {

template <class Container>
class Memory : public input::InputMemory<Container>, public output::OutputMemory<Container>
{
  public:
    explicit Memory( Container& buff ) :
        input::InputMemory<Container>( buff ), output::OutputMemory<Container>( buff ) {}
};

} // namespace io
} // namespace hub
