
#pragma once

#include "base/Macros.hpp"


#include "base/traits/SizeOf.hpp"
#include "base/traits/TypeId.hpp"
#include "base/traits/TypeName.hpp"
#include "base/traits/ToString.hpp"
#include "base/Traits.hpp"

#include "base/Defines.hpp"


#if CPP_VERSION <= 14
//#include "std_any/any.h"
#else
#    include <any>
#endif
