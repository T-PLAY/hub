/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright © 2021-2024 Hub. All Rights Reserved.
 * @author Gauthier Bouyjou <gauthierbouyjou@aol.com>
 * @date 2023/10/01
 */

#pragma once

#include <string>


namespace hub {

/// \brief s_contributionStart
extern const int s_contributionStart;

/// \brief s_contributionEnd
extern const int s_contributionEnd;

/** @brief Returns major library version */
extern const int s_versionMajor;
/** @brief Returns minor library version */
extern const int s_versionMinor;
/** @brief Returns revision field of the library version */
extern const int s_versionPatch;
/** @sa MajorVersion.MinorVersion.PatchVersion */
extern const std::string s_version;

/// \brief s_commitHash
extern const std::string s_commitHash;

// Doxygen sorted

///
/// \brief The client namespace
/// represents all instance able to connect to server
///
namespace client {}

///
/// \brief The data namespace
/// represents useful data (metadata) for user (not a sensor data)
///
namespace data {}

///
/// \brief The format namespace
/// represents all data comming from sensor (sensor specific)
///
namespace format {}

///
/// \brief The input namespace
/// represents all input (only) implements
///
namespace input {}

///
/// \brief The io namespace
/// represents all input/output (both) implements
///
namespace io {}

///
/// \brief The net namespace
/// defined socket communication (client/server sockets)
///
namespace net {}

///
/// \brief The output namespace
/// represents all output (only) implements
///
namespace output {}

///
/// \brief The sensor namespace
/// declare sensor abilities
///
namespace sensor {}

///
/// \brief The serializer namespace
/// is a collection of serializer able to serialize through network useful data (metadata)
///
namespace serializer {}

/// @brief The native namespace
/// Export these "C" functions usable by Unity C++ plugin or Matlab (loadlibrary).
///
namespace native {}

///
/// \brief The server namespace
/// defined server implementation able to share data from sensor to differents viewer applications
///
namespace server {}

} // namespace hub
