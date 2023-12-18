

#include "ios.hpp"

#include <mutex>

namespace hub {

// #if defined( HUB_DEBUG_INPUT ) || defined( HUB_DEBUG_OUTPUT )
std::mutex s_mtxIoPrint;
// #endif

} // namespace hub
