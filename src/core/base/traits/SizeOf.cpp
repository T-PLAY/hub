#include "SizeOf.hpp"

//#include <string>
//#include <string_view>
#include <cmath>

namespace hub {

//// source : https://www.mbeckler.org/blog/?p=114
//// Prints to the provided buffer a nice number of bytes (KB, MB, GB, etc)
std::string pretty_bytes(hub::Size_t bytes) {
    // std::string str;

    constexpr auto buffSize = 32;
    char buff[buffSize] { 0 };
    static CONSTEXPR17 std::string_view suffixes[] { "Bytes", "Ko", "Mo", "Go", "To", "Po", "Eo" };
    uint64_t s          = 0; // which suffix to use
    double count        = bytes;
    constexpr auto kilo = 1'000;

    while ( count >= kilo && s < 7 ) {
        s++;
        count /= kilo;
    }

    /// \brief count
    if ( count - floor( count ) == 0.0 ) {
#ifdef WIN32
        snprintf( buff, buffSize, "%d %s", (int)count, suffixes[s].data() );
#else
#    ifdef OS_MACOS
        snprintf( buff, buffSize, "%d %s", (int)count, suffixes[s].data() );
#    else
        sprintf( buff, "%d %s", (int)count, suffixes[s].data() );
#    endif
#endif
    }
    /// \brief else
    else
    {
#ifdef WIN32
        snprintf( buff, buffSize, "%.1f %s", count, suffixes[s].data() );
#else
#    ifdef OS_MACOS
        snprintf( buff, buffSize, "%.1f %s", count, suffixes[s].data() );
#    else
        sprintf( buff, "%.1f %s", count, suffixes[s].data() );
#    endif
#endif
    }

    // return std::string( buff );
    /// \brief return
    return buff;
}

}
