#include "Anyable.hpp"

// #include <cstring>
// #include <iostream>
// #include <map>
// #include <typeinfo>
// #include <vector>

// #include "data/Mat4.hpp"
// #include "data/Mesh.hpp"

namespace hub {
//namespace anyable {

//std::map<size_t, Anyable::AnyHelper> Anyable::s_anyables = makeAnyHelperMap<void, int, double, std::string, const char*>();
std::map<decltype(typeid(void).hash_code()), Anyable::AnyHelper> Anyable::s_anyables = makeAnyHelperMap<void>();

//} // namespace Anyable
} // namespace hub
