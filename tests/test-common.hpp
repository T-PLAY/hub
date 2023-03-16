

#include <ctime>
#include <random>

#include <Acquisition.hpp>

static int getRandomPort() {
    srand( (unsigned)time( NULL ) );
    return 6000 + rand() % ( 65535 - 6000 );
}

static int computeDist(const hub::Acquisition & acq, const hub::Acquisition & acq2) {
    return std::abs(acq.getStart() - acq2.getStart());
}
