
#include <cassert>
#include <string>

int main() {

    char magicNumber[80] = { 0 };
#ifdef WIN32
    sprintf_s( magicNumber, "%c%c%c %d.%d.%d", 'H', 'U', 'B', 1, 1, 0 );
#else
    sprintf( magicNumber, "%c%c%c %d.%d.%d", 'H', 'U', 'B', 1, 1, 0 );
#endif

    printf( "%s\n", magicNumber );

    int versionMajor;
    int versionMinor;
    int versionPatch;
    char h;
    char u;
    char b;
#ifdef WIN32
    sscanf_s( magicNumber,
              "%c%c%c %d.%d.%d",
              &h,
              1,
              &u,
              1,
              &b,
              1,
              &versionMajor,
              &versionMinor,
              &versionPatch );
#else
    sscanf( magicNumber,
            "%c%c%c %d.%d.%d",
            &h,
            1,
            &u,
            1,
            &b,
            1,
            &versionMajor,
            &versionMinor,
            &versionPatch );
#endif

    assert( h == 'H' );
    assert( u == 'U' );
    assert( b == 'B' );
    assert( versionMajor == 1 );
    assert( versionMinor == 1 );
    assert( versionPatch == 0 );

    return 0;
}
