

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
    for ( const auto& [streamName, sensorSpec] : listStreams ) {
        std::cout << streamName << std::endl;

        std::cout << sensorSpec << std::endl;

        auto acq = asker.getAcquisition( streamName );
        std::cout << acq << std::endl;

        std::cout << "--------------------------------" << std::endl;
    }

    return 0;
}
