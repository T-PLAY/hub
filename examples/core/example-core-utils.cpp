



#include <iostream>

#include <core/Utils.hpp>


int main( int argc, char* argv[] ) {

       bool exit = false;
       while ( !exit ) {                             // ESC to quit
           std::cout << "waiting key ..." << std::endl;
           const auto key = hub::utils::key_press(); // blocks until a key is pressed
           std::cout << "input key : " << key << std::endl;

//            switch ( key ) {
//            case hub::utils::Key::F5:
//            case hub::utils::Key::Space:
//                break;
//            case hub::utils::Key::Escape:
//            case hub::utils::Key::Dot:
//                exit = true;
//                break;
//            case hub::utils::Key::h:
//                break;
//            default:
//                std::cout << "unrecognized key : " << key << std::endl;
//            }
       }
       std::cout << "exiting" << std::endl;
}
