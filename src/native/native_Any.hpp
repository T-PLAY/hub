#pragma once

#include "core/Macros.hpp"

// #include "core/Any.hpp"

namespace hub {

class Any;

///
/// @brief native
/// Export these "C" functions usable by Unity C++ plugin or Matlab (loadlibrary).
///
namespace native {

#ifdef __cplusplus
extern "C"
{
#endif

    ///
    /// \brief any_getDouble
    /// \param any
    /// \return
    ///
    SRC_API double any_getDouble( const Any* any );

    ///
    /// \brief any_getInt
    /// \param any
    /// \return
    ///
    SRC_API int any_getInt( const Any* any );

#ifdef __cplusplus
} // end extern "C"
#endif

} // namespace native
} // namespace hub
