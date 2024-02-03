/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/10/01

#pragma once

#include <string>

/* cppcheck-suppress missingInclude */

namespace hub {

extern const int s_contributionStart;
extern const int s_contributionEnd;

/** @brief Returns major library version */
extern const int s_versionMajor;
/** @brief Returns minor library version */
extern const int s_versionMinor;
/** @brief Returns revision field of the library version */
extern const int s_versionPatch;
/** @sa MajorVersion.MinorVersion.PatchVersion */
extern const std::string s_version;

extern const std::string s_commitHash;

namespace core {}

namespace net {}

namespace io {}

namespace server {}

namespace client {}

namespace native {}

namespace sensor {}

namespace data {}

namespace server {}

} // namespace hub
