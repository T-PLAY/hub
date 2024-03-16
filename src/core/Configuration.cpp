#include "Configuration.hpp"

namespace hub {

#ifdef SRC_STATIC
const int s_servicePort         = 4042;
const std::string s_serviceIpv4 = "127.0.0.1";
const std::string s_extension   = "hub";
#endif

} // namespace hub
