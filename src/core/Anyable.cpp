#include "Anyable.hpp"

// #ifndef HUB_NON_BUILD_SENSOR
// #include "sensor/Format.hpp"
// #endif
#include "core/Format.hpp"

#ifndef HUB_NON_BUILD_DATA
#include "data/Mesh.hpp"
#endif

namespace hub {

// std::map<size_t, Anyable::AnyHelper> Anyable::s_anyables = makeAnyHelperMap<void, int, double,
// std::string, const char*>();

using namespace format;

#ifndef HUB_NON_BUILD_DATA
// using namespace sensor::format;
// using namespace data;
std::map<decltype( typeid( void ).hash_code() ), Anyable::AnyHelper> Anyable::s_anyables =
    makeAnyHelperMap<void, int, double, std::string, const char*, format::Mat4, data::Mesh>();
#else
std::map<decltype( typeid( void ).hash_code() ), Anyable::AnyHelper> Anyable::s_anyables =
    // todo c++17
    // makeAnyHelperMap<void, int, double, std::string, const char*, Mat4>();
    // makeAnyHelperMap<void, int, double, std::string, const char*>();
    makeAnyHelperMap<int, double, std::string, const char*>();
#endif

} // namespace hub
