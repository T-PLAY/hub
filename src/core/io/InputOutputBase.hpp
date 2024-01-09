/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/09
	
#pragma once

//#include "core/Macros.hpp"
//#include "input/BasicInputI.hpp"
//#include "output/BasicOutputI.hpp"
#include "core/InputBase.hpp"
#include "core/OutputBase.hpp"

namespace hub {
namespace io {

class InputOutputBase : public InputBase, public OutputBase
{
  public:
};

} // namespace io
} // namespace hub
