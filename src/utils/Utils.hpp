#pragma once

//#include <iostream>
//#include <memory>
#include <string>
#include <cassert>
#ifndef WIN32
#    include <unistd.h>
#else
#include <winsock2.h>
#include <iostream>
//#include <WinBase.h>
#endif

//#include "Macros.hpp"

namespace hub {
namespace utils {

static std::string getHostname() {
    char* temp = 0;
    std::string computerName;

    

#if defined( WIN32 ) || defined( _WIN32 ) || defined( _WIN64 )
//    temp = getenv( "COMPUTERNAME" );
    char buff[256];
    //LPDWORD len;
    //int err = _dupenv_s((char**)&buff, &len, "COMPUTERNAME");
    //int err = gethostname( buff, 256 );
    //GetComputerNameA( buff, &len );

    WORD wVersionRequested;
    WSADATA wsaData;
    char name[255];
    //CString ip;
    std::string ip;
    PHOSTENT hostinfo;
    wVersionRequested = MAKEWORD( 2, 0 );

    if ( WSAStartup( wVersionRequested, &wsaData ) == 0 ) {
        if ( gethostname( name, sizeof( name ) ) != 0 ) {
            assert( false );
            //if ( ( hostinfo = gethostbyname( name ) ) != NULL ) {
            //    ip = inet_ntoa( *(struct in_addr*)*hostinfo->h_addr_list );
            //}
        }
        WSACleanup();
    }

    //std::cout << "hostname = " << name << std::endl;

   
    //assert(len < 256);
    //assert(err == 0);
    computerName = name;
//    if ( temp != 0 ) {
//        computerName = temp;
//        temp         = 0;
//    }
#else
    temp = getenv( "HOSTNAME" );
    if ( temp != 0 ) {
        computerName = temp;
        temp         = 0;
    }
    else {
        temp = new char[512];
        if ( gethostname( temp, 512 ) == 0 ) { // success = 0, failure = -1
            computerName = temp;
        }
        delete[] temp;
        temp = 0;
    }
#endif
    return computerName;
}


} // namespace utils
} // namespace hub
