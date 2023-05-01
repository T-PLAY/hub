
#pragma once

//#include "Version.h"

#include <string>

//#define HUB_VERSION_MAJOR 1
//#define HUB_VERSION_MINOR 0
//#define HUB_VERSION_PATCH 0
//#define HUB_VERSION "1.0.0"

///
/// @author Gauthier Bouyjou
/// @date 2021-2023
/// If you have any questions please send me message to my mailbox.
/// @email : gauthierbouyjou@aol.com
/// @brief
///
namespace hub {

/** @brief Returns major library version */
static constexpr int s_versionMajor = HUB_VERSION_MAJOR;
/** @brief Returns minor library version */
static constexpr int s_versionMinor = HUB_VERSION_MINOR;
/** @brief Returns revision field of the library version */
static constexpr int s_versionPatch = HUB_VERSION_PATCH; // Revision
/** @sa getMajorVersion, getMinorVersion, getRevisionVersion */
static const std::string s_version = HUB_VERSION;

} // namespace hub
