

#include <ctime>
#include <random>

#include <Acquisition.hpp>

static int getRandomPort( const char* filename ) {
    srand( (unsigned)time( NULL ) );
    constexpr int offset = 6000;
    const unsigned int random       = std::hash<std::string>()( filename ) + rand();
//    std::cout << "random : " << random << std::endl;
    const unsigned int ret = offset + random % ( 65535 - offset );
    assert(offset <= ret && ret <65536);
    std::cout << "use port number: " << ret << std::endl;
    return ret;
}

static int computeDist( const hub::Acquisition& acq, const hub::Acquisition& acq2 ) {
    return std::abs( acq.getStart() - acq2.getStart() );
}
