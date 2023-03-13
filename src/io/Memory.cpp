#include "io/Memory.hpp"

#include <cassert>
#include <iostream>

#include <cstring>
#include <thread>

namespace hub {
namespace io {


//Memory::Memory( Container& buff ) : m_container( buff ) {}

//void Memory::close() const {
////    assert( isOpen() );
////    m_container.close();
////    assert( !isOpen() );
//}

//bool Memory::isOpen() const {
////    return m_container.isOpen();
//}

//bool Memory::isEnd() const {
////    assert( isOpen() );
////    return m_container.isEnd();
//}

//void Memory::write( const unsigned char* data, size_t len ) const {
////    m_container.write( data, len );
//}

//void Memory::read( unsigned char* data, size_t len ) const {
////    try {
////        m_container.read( data, len );
////    }
////    catch ( std::exception& e ) {
////        std::cout << "[Memory] catch exception : " << e.what() << std::endl;
////        throw e;
////    }
//}

} // namespace io
} // namespace hub
