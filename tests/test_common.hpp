
#include <catch2/catch_test_macros.hpp>

#include <ctime>
#include <random>

#include <Acquisition.hpp>

#define GET_RANDOM_PORT getRandomPort( __FILE__ )

static int getRandomPort( const char* filename ) {
    srand( (unsigned)time( NULL ) );
    constexpr int offset      = 6000;
    const unsigned int random = std::hash<std::string>()( filename ) + rand();
    const unsigned int ret    = offset + random % ( 65535 - offset );
    assert( offset <= ret && ret < 65536 );
    return ret;
}

static int computeDist( const hub::Acquisition& acq, const hub::Acquisition& acq2 ) {
    return std::abs( acq.getStart() - acq2.getStart() );
}

static void checkRatio(double ratio, int compare) {
    const int gap = 5;
    CHECK((compare - gap <= ratio && ratio <= compare + gap));
    if (! (compare - gap <= ratio && ratio <= compare + gap)) {
        std::cout << "---------------------------------------------------------------------------------------------------------------------> checkRatio: " << compare - gap << " <= " << ratio << " <= " << compare + gap << std::endl;
    }
}
