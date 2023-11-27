
#include <iomanip>
#include <iostream>

#include "Client2.hpp"
#include "ServerImpl2.hpp"

namespace hub {
namespace server {
namespace impl2 {

Client2::Client2( ServerImpl2* server, int iClient ) : m_server( server ), m_iClient( iClient ) {
    assert( m_server != nullptr );
}

Client2::~Client2() {
    if ( m_server != nullptr ) m_server->removeClient( this );
}

constexpr int g_margin  = 45;
constexpr int g_margin2 = 20;

std::string Client2::headerMsg() const {
    const std::string str = "\t\033[" + std::to_string( 31 + m_iClient % 7 ) + "m" +
                            "[Client2:" + std::to_string( m_iClient ) + "/" +
                            std::to_string( m_server->m_nActiveClient ) + "]\033[0m";
    return str;
}

void Client2::printStatusMessage( const std::string& message ) const {
    if ( m_server == nullptr ) {
        std::cout << std::left << std::setw( g_margin ) << headerMsg() << std::setw( g_margin2 )
                  << message << "status : server exited" << std::endl
                  << "-------------------------------------------------------------------------"
                     "--------------------"
                  << std::endl;
    }
    else {
        std::cout << std::left << std::setw( g_margin ) << headerMsg() << std::setw( g_margin2 )
                  << message << m_server->getStatus() << std::endl
                  << "-------------------------------------------------------------------------"
                     "--------------------"
                  << std::endl;
    }
}

void Client2::setServer( ServerImpl2* newServer ) {
    assert( m_server != nullptr );
    m_server = newServer;
}

} // namespace server
} // namespace impl2
} // namespace hub
