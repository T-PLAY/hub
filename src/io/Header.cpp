#include "Header.hpp"

#include <array>

#include "core/Info.hpp"
#include "core/Macros.hpp"
#include "core/Traits.hpp"
#include "core/io/Memory.hpp"

namespace hub {
namespace io {

Header::Header( Size_t dataSize, const Datas_t& userDefined ) :
    m_dataSize( dataSize ), m_userDefined( userDefined ) {
    m_headerSize = getSize();

//    const auto joker = ' ';

#ifdef WIN32
    sprintf_s( m_magicNumber.data(),
               m_magicNumber.size(),
//               "%c%c%c %d.%d.%d * %s",
               "%c%c%c %d.%d.%d",
               'H',
               'U',
               'B',
               s_versionMajor,
               s_versionMinor,
               s_versionPatch
//               std::string( "Copyright " + std::to_string( s_contributionStart ) + "-" +
//                            std::to_string( s_contributionEnd ) +
//                            "* author : gauthierbouyjou@aol.com" )
//                   .c_str()
              );
#else
#    ifdef OS_MACOS
    snprintf( m_magicNumber.data(),
             m_magicNumber.size(),
//              "%c%c%c %d.%d.%d * %s",
              "%c%c%c %d.%d.%d",
              'H',
              'U',
              'B',
              s_versionMajor,
              s_versionMinor,
              s_versionPatch
//              std::string( "Copyright " + std::to_string( s_contributionStart ) + "-" +
//                           std::to_string( s_contributionEnd ) +
//                           "* author : gauthierbouyjou@aol.com" )
//                  .c_str()
             );
#    else
    sprintf( m_magicNumber.data(),
             // "%c%c%c %d.%d.%d * %s",
             "%c%c%c %d.%d.%d",
             'H',
             'U',
             'B',
             s_versionMajor,
             s_versionMinor,
             s_versionPatch
             // std::string( "Copyright " + std::to_string( s_contributionStart ) + "-" +
                          // std::to_string( s_contributionEnd ) +
                          // "* author : gauthierbouyjou@aol.com" )
                 // .c_str()
             );
#    endif
#endif

    const auto joker = '_';
    const auto strLen = strlen(m_magicNumber.data());
    assert( strLen < sizeof( m_magicNumber ) );
//    std::fill( m_magicNumber.begin(), m_magicNumber.end(), joker );
    std::fill( m_magicNumber.begin() + strLen, m_magicNumber.end(), joker );
    m_magicNumber.back() = '\n';

//    m_magicNumber[strlen( m_magicNumber.data() )] = joker;
}

void Header::checkMagicNumber() const {

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
}

} // namespace io
} // namespace hub
