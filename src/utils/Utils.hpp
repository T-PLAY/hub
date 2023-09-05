#pragma once

//#include <iostream>
//#include <memory>
#include <string>
#include <cassert>
#ifndef WIN32
#    include <unistd.h>
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
    size_t len;
    int err = _dupenv_s((char**)&buff, &len, "COMPUTERNAME");
    assert(len < 256);
    assert(err == 0);
    computerName = buff;
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
