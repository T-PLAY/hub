/*******************************************************************************
* Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
* Please visit https://www.irit.fr/tplay/.
*
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Mozilla Public License, v. 2.0
* available at https://mozilla.org/MPL/2.0/.
*
* Initial Contributors:
*   - Gauthier Bouyjou
*   - François Gaits (IRIT)
*******************************************************************************/

#pragma once

#include <string>

namespace hub {

#ifdef SRC_STATIC
extern const int s_servicePort;
extern const std::string s_serviceIpv4;
extern const std::string s_extension;
#else
static const int s_servicePort         = 4042;
static const std::string s_serviceIpv4 = "127.0.0.1";
static const std::string s_extension = "hub";
#endif

} // namespace hub

#ifndef HUB_SERVICE_PORT
#    define HUB_SERVICE_PORT hub::s_servicePort
#endif

#ifndef HUB_SERVICE_IPV4
#    define HUB_SERVICE_IPV4 hub::s_serviceIpv4
#endif

#ifndef HUB_EXTENSION
#    define HUB_EXTENSION hub::s_extension
#endif
