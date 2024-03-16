
#include "io/output/OutputStream.hpp"

#define HUB_CPP_SOURCE
#include "native_OutputStream.hpp"

#include <cstring>

namespace hub {
namespace native {

output::OutputStream*
createOutputStream( const io::Header* header, const char* streamName, int port, const char* ipv4 ) {

    auto* outputStream = new output::OutputStream( *header, streamName, port, ipv4 );
    return outputStream;
}

void freeOutputStream( output::OutputStream* outputStream ) {
    assert( outputStream != nullptr );
    delete outputStream;
    // Todo header = nullptr
}

void outputStream_write_int( output::OutputStream* outputStream, int value ) {
    outputStream->write( value );
}

} // namespace native
} // namespace hub
