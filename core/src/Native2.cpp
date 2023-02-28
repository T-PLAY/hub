#include "Native2.hpp"

/* Native.cpp : Defines the exported functions for the DLL.
 */

#include <iostream>

//#include <cstring>
//#include <sstream>

// namespace hub {
// namespace native2 {

// void createInputSensor( const char* streamName, const char* ipv4, int port ) {
//    std::cout << "[Native] createInputSensor( " << streamName << ")" << std::endl;
//    InputSensor* inputSensor = nullptr;
//    try {
//        inputSensor =
//            new InputSensor( io::InputStream( streamName, "", net::ClientSocket( ipv4, port ) ) );
//    }
//    catch ( std::exception& e ) {
//        std::cout << "[Native] createInputSensor : catch exception : " << e.what() << std::endl;
//        return nullptr;
//    }
//    return inputSensor;
//}

//} // namespace native2
//} // namespace hub
