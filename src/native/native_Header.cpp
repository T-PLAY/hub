
#include "io/Header.hpp"

#define HUB_CPP_SOURCE
#include "native_Header.hpp"

// #include <cstring>

namespace hub {
namespace native {

io::Header*
createHeader( Size_t dataSize) {

    auto* header = new io::Header(dataSize);
    return header;
}

void freeHeader( io::Header* header ) {
    assert( header != nullptr );
    delete header;
    header = nullptr;
}

// void outputStream_write_int(output::OutputStream *outputStream, int value)
// {
//     outputStream->write(value);
// }

} // namespace native
} // namespace hub
