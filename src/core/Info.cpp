#include "Info.hpp"

#include <string>

// Include all core headers to check dll export
#include "Base.hpp"
#include "Any.hpp"
#include "Anyable.hpp"
#include "Buffer.hpp"
#include "Configuration.hpp"
#include "Format.hpp"
#include "Info.hpp"
#include "Input.hpp"
#include "InputBase.hpp"
#include "Matrix.hpp"
#include "MatrixBase.hpp"
#include "MetaData.hpp"
#include "Node.hpp"
#include "Output.hpp"
#include "OutputBase.hpp"
#include "Serializer.hpp"
#include "Types.h"
#include "Utils.hpp"
#include "Version.hpp"
#include "ios.hpp"

#include "matrix/MatrixT.hpp"
#include "matrix/MatrixTs.hpp"
#include "matrix/MatrixXD.hpp"

#include "Resolution.hpp"

#include "io/Archive.hpp"
#include "io/InputOutput.hpp"
#include "io/Memory.hpp"

#include "serializer/SerializerBoost.hpp"
#include "serializer/SerializerI.hpp"
#include "serializer/SerializerImpl.hpp"
#include "serializer/SerializerImpl2.hpp"
#include "serializer/SerializerZpp.hpp"
#include "serializer/SerializerZppBits.hpp"


namespace hub {

const int s_contributionStart  = HUB_CONTRIBUTION_START;
const int s_contributionEnd    = HUB_CONTRIBUTION_END;
const int s_versionMajor       = HUB_VERSION_MAJOR;
const int s_versionMinor       = HUB_VERSION_MINOR;
const int s_versionPatch       = HUB_VERSION_PATCH; // Revision
const std::string s_version    = HUB_VERSION;
const std::string s_commitHash = HUB_COMMIT_HASH;

} // namespace hub
