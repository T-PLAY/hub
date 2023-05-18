
#include <string>
#include <cassert>

int main() {

    char magicNumber[80] = {0};
//    constexpr char joker = ' ';
//    memset(magicNumber, joker, 79);
#ifdef WIN32
    sprintf_s (magicNumber, "%c%c%c %d.%d.%d", 'H', 'U', 'B', 1, 1, 0);
#else
    sprintf (magicNumber, "%c%c%c %d.%d.%d", 'H', 'U', 'B', 1, 1, 0);
#endif

    printf("%s\n", magicNumber);

//    char magicNumber[80] = {0};
//    read((unsigned char*)magicNumber, 80);
    int versionMajor;
    int versionMinor;
    int versionPatch;
    char h;
    char u;
    char b;
//    wchar_t wc;
#ifdef WIN32
    sscanf_s(magicNumber, "%c%c%c %d.%d.%d", &h, 1, &u, 1, &b, 1, &versionMajor, &versionMinor, &versionPatch);
#else
    sscanf(magicNumber, "%c%c%c %d.%d.%d", &h, 1, &u, 1, &b, 1, &versionMajor, &versionMinor, &versionPatch);
#endif
//    sscanf_s(magicNumber, "%c%c%c %d.%d.%d", &h, &u, &b, &versionMajor, &versionMinor, &versionPatch, (unsigned)_countof(magicNumber));


    assert(h == 'H');
    assert(u == 'U');
    assert(b == 'B');
    assert(versionMajor == 1);
    assert(versionMinor == 1);
    assert(versionPatch == 0);

    return 0;
}
