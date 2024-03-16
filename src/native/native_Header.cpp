
#include "io/Header.hpp"

#define HUB_CPP_SOURCE
#include "native_Header.hpp"

namespace hub {
namespace native {

io::Header* createHeader( Size_t dataSize ) {

    auto* header = new io::Header( dataSize );
    return header;
}

void freeHeader( io::Header* header ) {
    assert( header != nullptr );
    delete header;
    // Todo header = nullptr
}

} // namespace native
} // namespace hub
