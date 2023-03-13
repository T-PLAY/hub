

#include <random>
#include <ctime>


static int getRandomPort() {
    srand( (unsigned)time( NULL ) );
    return 6000 + rand() % (65535 - 6000);
}
