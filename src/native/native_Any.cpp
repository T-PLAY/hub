
#include "native_Any.hpp"

namespace hub {
namespace native {

double any_getDouble( const Any* any ) {
    return any->get<double>();
}

int any_getInt( const Any* any ) {
    return any->get<int>();
}

} // namespace native
} // namespace hub
