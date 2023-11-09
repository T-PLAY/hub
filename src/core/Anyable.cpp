#include "Anyable.hpp"

namespace hub {

// std::map<size_t, Anyable::AnyHelper> Anyable::s_anyables = makeAnyHelperMap<void, int, double,
// std::string, const char*>();

std::map<decltype( typeid( void ).hash_code() ), Anyable::AnyHelper> Anyable::s_anyables =
    makeAnyHelperMap<void>();

} // namespace hub
