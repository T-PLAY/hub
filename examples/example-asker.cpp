

// #include <stdio.h>
// #include <cstdio>
// #include <stdlib.h>
// #include <stdio.h>
// #include <iostream>
//#include <Viewer.hpp>
//#include <chrono>
#include <thread>

#include <client/Asker.hpp>

/// \file
/// \warning
/// Needs running server.

int main() {

    hub::client::Asker asker;

    auto listStreams = asker.listStreams();

    std::cout << "nb stream : " << listStreams.size() << std::endl;
#if ( __cplusplus >= 201703L )
    for ( const auto& [streamName, sensorSpec] : listStreams ) {
#else
    for ( const auto& pair : listStreams ) {
        const auto & streamName = pair.first;
        const auto & sensorSpec = pair.second;
#endif
        std::cout << streamName << std::endl;

        std::cout << sensorSpec << std::endl;

        auto acq = asker.getAcquisition( streamName );
        std::cout << acq << std::endl;

        std::cout << "--------------------------------" << std::endl;
    }

    return 0;
}
