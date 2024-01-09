#include "Header.hpp"

// #include "io/output/OutputBase.hpp"

// #include "io/output/OutputImpl.hpp"
// #include "io/output/OutputZppBits.hpp"
#include <array>

#include "core/Info.hpp"
#include "core/Macros.hpp"
#include "core/Traits.hpp"
#include "core/io/Memory.hpp"
// #include "ios.hpp"

namespace hub {
namespace io {

Header::Header( Size_t dataSize, const Datas_t& userDefined ) :
    m_dataSize( dataSize ), m_userDefined( userDefined ) {
    m_headerSize = getSize();

    // memset(m_magicNumber.data(), ' ', 31);
    const auto joker = ' ';
    std::fill( m_magicNumber.begin(), m_magicNumber.end(), joker );
    m_magicNumber.back() = '\n';
    // m_magicNumber.back() = 0;

#ifdef WIN32
    sprintf_s( m_magicNumber.data(),
               128,
               "%c%c%c %d.%d.%d * %s",
               'H',
               'U',
               'B',
               s_versionMajor,
               s_versionMinor,
               s_versionPatch,
             std::string("Copyright " + std::to_string(s_contributionStart) + "-" + std::to_string(s_contributionEnd) + "* author : gauthierbouyjou@aol.com").c_str() );
#else
#    ifdef OS_MACOS
    snprintf( m_magicNumber.data(),
              128,
              "%c%c%c %d.%d.%d * %s",
              'H',
              'U',
              'B',
              s_versionMajor,
              s_versionMinor,
              s_versionPatch,
             std::string("Copyright " + std::to_string(s_contributionStart) + "-" + std::to_string(s_contributionEnd) + "* author : gauthierbouyjou@aol.com").c_str() );
#    else
    sprintf( m_magicNumber.data(),
             "%c%c%c %d.%d.%d * %s",
             'H',
             'U',
             'B',
             s_versionMajor,
             s_versionMinor,
             s_versionPatch,
             std::string("Copyright " + std::to_string(s_contributionStart) + "-" + std::to_string(s_contributionEnd) + "* author : gauthierbouyjou@aol.com").c_str() );
#    endif
#endif

    assert( strlen( m_magicNumber.data() ) < sizeof( m_magicNumber ) );
    m_magicNumber[strlen( m_magicNumber.data() )] = joker;
    //    memory.write( reinterpret_cast<unsigned char*>(m_magicNumber), 80 );
    // write( reinterpret_cast<unsigned char*>( m_magicNumber ), 80 );
}

// todo hub2
void Header::checkMagicNumber() const {
    // input.read( (Data_t*)m_magicNumber.data(), m_magicNumber.size() );

    int versionMajor;
    int versionMinor;
    int versionPatch;
    char h;
    char u;
    char b;
#ifdef WIN32
    sscanf_s( m_magicNumber.data(),
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
    sscanf( m_magicNumber.data(),
            "%c%c%c %d.%d.%d",
            &h,
            &u,
            &b,
            &versionMajor,
            &versionMinor,
            &versionPatch );
#endif
    assert( h == 'H' );
    assert( u == 'U' );
    assert( b == 'B' );
    assert( versionMajor <= s_versionMajor );
    assert( versionMinor <= s_versionMinor );
    assert( versionPatch <= s_versionPatch );

    // input.readAll( m_headerSize, m_dataSize, m_userDefined );

    // #ifdef DEBUG
    //     std::cout <<  "[InputImpl] read(magic number) : '" << magicNumber << "'" <<
    //     std::endl;
    // #endif

    // assert( m_magicNumber[0] == 'H' );
    // assert( m_magicNumber[1] == 'U' );
    // assert( m_magicNumber[2] == 'B' );
    // assert( m_magicNumber[3] <= s_versionMajor );
    // assert( m_magicNumber[4] <= s_versionMinor );
    // assert( m_magicNumber[5] <= s_versionPatch );
}

} // namespace io
} // namespace hub
