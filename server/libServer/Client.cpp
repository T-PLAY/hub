
#include <iomanip>
#include <iostream>

#include "Client.hpp"
#include "Server.hpp"

std::mutex Client::s_mtxCout;

Client::Client( Server& server, int iClient ) : m_server( server ), m_iClient( iClient ) {}

constexpr int g_margin  = 45;
constexpr int g_margin2 = 20;

std::string Client::headerMsg() const {
    const std::string str =
        "\t\033[" + std::to_string( 31 + m_iClient % 7 ) + "m" +
        //    const std::string str = "\033[" + std::to_string( 31 + m_iClient % 7 ) + "m" +
        "[Client:" + std::to_string( m_iClient ) + "]\033[0m";
    return str;
}

void Client::printStatusMessage( const std::string& message ) const {
    std::cout << std::left << std::setw( g_margin ) << headerMsg() << std::setw( g_margin2 )
              << message << m_server.getStatus() << std::endl
              << "-------------------------------------------------------------------------"
                 "--------------------"
              << std::endl;
}
