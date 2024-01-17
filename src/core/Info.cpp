#include "Info.hpp"

#include <string>

#include "Version.hpp"

// check all core headers compilation

// level 0
#include "Any.hpp"
#include "Anyable.hpp"
#include "Buffer.hpp"
#include "Configuration.hpp"
#include "Format.hpp"
#include "Info.hpp"
#include "Input.hpp"
#include "InputBase.hpp"
#include "ios.hpp"
#include "Macros.hpp"
#include "MetaData.hpp"
#include "Node.hpp"
#include "Output.hpp"
#include "OutputBase.hpp"
#include "Serializer.hpp"
#include "Traits.hpp"
#include "Utils.hpp"
#include "MatrixBase.hpp"
#include "Matrix.hpp"
#include "Types.h"

// matrix
#include "matrix/MatrixXD.hpp"
#include "matrix/MatrixTs.hpp"
#include "matrix/MatrixT.hpp"

// level 0
#include "Resolution.hpp"

// io
#include "io/Archive.hpp"
#include "io/book/BookImpl.hpp"
#include "io/book/BookZppBits.hpp"
#include "io/Book.hpp"
#include "io/input/InputImpl.hpp"
#include "io/input/InputZppBits.hpp"
#include "io/InputOutputBase.hpp"
#include "io/InputOutput.hpp"
#include "io/Memory.hpp"
#include "io/output/OutputImpl.hpp"
#include "io/output/OutputZppBits.hpp"

// serializer
#include "serializer/SerializerI.hpp"
#include "serializer/SerializerImpl.hpp"
#include "serializer/SerializerZppBits.hpp"
#include "serializer/SerializerImpl2.hpp"
#include "serializer/SerializerZpp.hpp"
#include "serializer/SerializerBoost.hpp"

// traits
#include "traits/Array.hpp"
#include "traits/Map.hpp"
#include "traits/Span.hpp"
#include "traits/std_any.hpp"
#include "traits/Tuple.hpp"
#include "traits/Vector.hpp"

namespace hub {

const int s_contributionStart = HUB_CONTRIBUTION_START;
const int s_contributionEnd = HUB_CONTRIBUTION_END;
const int s_versionMajor = HUB_VERSION_MAJOR;
const int s_versionMinor = HUB_VERSION_MINOR;
const int s_versionPatch = HUB_VERSION_PATCH; // Revision
const std::string s_version = HUB_VERSION;
const std::string s_commitHash = HUB_COMMIT_HASH;

} // namespace hub
