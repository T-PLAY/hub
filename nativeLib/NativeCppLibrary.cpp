
/* NativeCppLibrary.cpp : Defines the exported functions for the DLL.
 */
// #include “pch.h”
// #include “framework.h”
//#include “NativeCppLibrary.h”
#include "NativeCppLibrary.h"
// Standard Library imports

#include <iostream>

int displayNumber() {
    return 1;
}

int getRandom() {
    //  return rand();
    std::cout << "getRandom" << std::endl;
    return 8;
}

int displaySum() {
    int first_number  = 7;
    int second_number = 7;
    int total         = first_number + second_number;
    return total;
}

class CNativeCppLibrary
{
  public:
    CNativeCppLibrary( void );
    // TODO: add your methods here.
    // Block declarations

    int print() {
        std::cout << "hello" << std::endl;
        return 5;
    }

    void goodBye();
    //    int displayNumber();
    //    NATIVECPPLIBRARY_API int displayNumber();
    //    NATIVECPPLIBRARY_API int getRandom();
    //    NATIVECPPLIBRARY_API int displaySum();
};

void CNativeCppLibrary::goodBye() {
    std::cout << "good bye" << std::endl;
}
