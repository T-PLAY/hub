#pragma once

#include <string>

/* cppcheck-suppress missingInclude */
// #include "Version.hpp"

///
/// @author Gauthier Bouyjou
/// @date 2022-2023
/// If you have any questions please do not hesitate to send me message to my mailbox.
/// email : gauthierbouyjou@aol.com
/// @brief
///
namespace hub {

/** @brief Returns major library version */
// static constexpr int s_versionMajor = HUB_VERSION_MAJOR;
extern const int s_versionMajor;
/** @brief Returns minor library version */
// static constexpr int s_versionMinor = HUB_VERSION_MINOR;
extern const int s_versionMinor;
/** @brief Returns revision field of the library version */
// static constexpr int s_versionPatch = HUB_VERSION_PATCH; // Revision
extern const int s_versionPatch;
/** @sa MajorVersion.MinorVersion.PatchVersion */
// static const std::string s_version = HUB_VERSION;
extern const std::string s_version;

extern const std::string s_commitHash;

/// @date

namespace core {}

///
/// @brief
///
namespace net {}

///
/// @brief
///
namespace io {
}

///
/// @brief
///
namespace server {}

///
/// @brief
///
namespace client {}

namespace native {}

namespace sensor {}

///
/// @brief
///
namespace data {}




} // namespace hub
