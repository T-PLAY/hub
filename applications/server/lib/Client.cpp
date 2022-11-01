
#include "Client.hpp"

Client::Client( Server& server, int iClient ) : m_server( server ), m_iClient( iClient ) {}

// Client::~Client() {
// }

std::string Client::headerMsg() const {
    const std::string str = "\t\033[" + std::to_string( 31 + m_iClient % 7 ) + "m" +
                            "[Client:" + std::to_string( m_iClient ) + "]\033[0m";
    return str;
}
