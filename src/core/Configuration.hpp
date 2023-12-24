
#pragma once

#include <string>

namespace hub {

// static constexpr int s_versionMajor = HUB_VERSION_MAJOR;
extern const int s_servicePort;
extern const std::string s_serviceIpv4;

}

#ifndef HUB_SERVICE_PORT
// #define HUB_SERVICE_PORT 4042
#define HUB_SERVICE_PORT hub::s_servicePort
#endif

#ifndef HUB_SERVICE_IPV4
// #define HUB_SERVICE_IPV4 "127.0.0.1"
#define HUB_SERVICE_IPV4 hub::s_serviceIpv4
#endif
