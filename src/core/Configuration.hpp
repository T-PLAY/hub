/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/12/13
	
#pragma once

#include <string>

namespace hub {

// static constexpr int s_versionMajor = HUB_VERSION_MAJOR;

#ifdef SRC_STATIC
extern const int s_servicePort;
extern const std::string s_serviceIpv4;
#else
static const int s_servicePort = 4042;
static const std::string s_serviceIpv4 = "127.0.0.1";
#endif

}

#ifndef HUB_SERVICE_PORT
// #define HUB_SERVICE_PORT 4042
#define HUB_SERVICE_PORT hub::s_servicePort
#endif

#ifndef HUB_SERVICE_IPV4
// #define HUB_SERVICE_IPV4 "127.0.0.1"
#define HUB_SERVICE_IPV4 hub::s_serviceIpv4
#endif
