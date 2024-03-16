

#include <iostream>

#include <core/Utils.hpp>

int main( int argc, char* argv[] ) {

    bool exit = false;
    while ( !exit ) { // ESC to quit
        std::cout << "waiting key ..." << std::endl;
        const auto key = hub::utils::key_press(); // blocks until a key is pressed
        std::cout << "input key : " << key << std::endl;
    }
    std::cout << "exiting" << std::endl;
}
