#include "SerializerImpl.hpp"

namespace hub {
namespace serializer {

void SerializerImpl::write( const std::string& str ) {
    //    assert( isOpen() );

#ifdef HUB_DEBUG_OUTPUT
    std::cout << HEADER << "write(" << TYPE_NAME( str ) << ") : '" << str << "'" << std::endl;
#endif

    int strLen = static_cast<int>( str.size() );
    assert( sizeof( int ) == 4 );
    write( strLen );

    if ( strLen > 0 ) {
        const char* data = str.data();
        write( reinterpret_cast<const unsigned char*>( data ), strLen );
    }
}

//void SerializerImpl::write( const char* str ) {
//    assert( str != nullptr );
//    //    assert( isOpen() );

//#ifdef HUB_DEBUG_OUTPUT
//    std::cout << HEADER << "write(" << TYPE_NAME( str ) << ")" << std::endl;
//#endif

//    uint32_t strLen = static_cast<int>( strlen( str ) );
//    write( strLen );

//    if ( strLen > 0 ) { write( reinterpret_cast<const unsigned char*>( str ), strLen ); }
//}

////////////////////////////////////////////////////////////////////////////

void SerializerImpl::read( std::string& str ) {
//    assert( isOpen() );
//    assert( !isEnd() );

    int strLen = 0;
    read( strLen );

    if ( strLen == 0 ) { str = ""; }
    else {
        const int buffSize = strLen + 1;
        unsigned char* tmp = new unsigned char[buffSize];
        read( tmp, strLen );
        tmp[strLen] = 0;
        str         = std::string( reinterpret_cast<char*>( tmp ) );
        delete[] tmp;
    }
#ifdef HUB_DEBUG_INPUT
    std::cout << "\t" << HEADER << "read(" << TYPE_NAME( str ) << ") : '" << str << "'"
              << std::endl;
#endif
}

//void SerializerImpl::read( char* str ) {
//    assert( str != nullptr );
////    assert( isOpen() );
////    assert( !isEnd() );

//#ifdef HUB_DEBUG_INPUT
//    std::cout << "\t" << HEADER << "read(" << TYPE_NAME( str ) << ")" << std::endl;
//#endif

//    int strLen = 0;
//    assert( sizeof( int ) == 4 );
//    read( strLen );

//    if ( strLen == 0 ) { str[0] = 0; }
//    else {
//        read( reinterpret_cast<unsigned char*>( str ), strLen );

//        str[strLen] = 0;
//    }
//}

} // namespace serializer
} // namespace hub
