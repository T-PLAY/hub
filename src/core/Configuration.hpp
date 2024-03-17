/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright © 2021-2024 Hub. All Rights Reserved.
 * @author Gauthier Bouyjou <gauthierbouyjou@aol.com>
 * @date 2023/12/13
 */

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
