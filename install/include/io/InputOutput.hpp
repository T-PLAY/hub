#pragma once

#include "Input.hpp"
#include "Output.hpp"

namespace hub {
namespace io {

///
/// \brief The InputOutput class
///
class SRC_API InputOutput : public Input, public Output
{
//  public:
//    InputOutput() = default;
//    ~InputOutput() {
//        if (Input::isOpen()) {
//            Input::close();
//        }
//        assert(! Input::isOpen());

//        if (Output::isOpen()) {
//            Output::close();
//        }
//        assert(! Output::isOpen());
//    }

};

} // namespace io
} // namespace hub
