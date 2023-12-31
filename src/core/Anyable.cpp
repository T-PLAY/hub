#include "Anyable.hpp"

#ifndef HUB_BUILD_NON_SENSOR
#include "sensor/Format.hpp"
#endif

namespace hub {

// std::map<size_t, Anyable::AnyHelper> Anyable::s_anyables = makeAnyHelperMap<void, int, double,
// std::string, const char*>();


#ifndef HUB_BUILD_NON_SENSOR
using namespace sensor::format;
std::map<decltype( typeid( void ).hash_code() ), Anyable::AnyHelper> Anyable::s_anyables =
    makeAnyHelperMap<void, int, double, std::string, const char*, Mat4>();
#else
std::map<decltype( typeid( void ).hash_code() ), Anyable::AnyHelper> Anyable::s_anyables =
    makeAnyHelperMap<void, int, double, std::string, const char*>();
#endif

} // namespace hub
