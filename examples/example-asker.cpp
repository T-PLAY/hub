

// #include <stdio.h>
// #include <cstdio>
// #include <stdlib.h>
// #include <stdio.h>
// #include <iostream>
//#include <Viewer.hpp>
//#include <chrono>
#include <thread>

#include <Asker.hpp>

/// \file
/// \warning
/// Needs running server.

int main() {

    hub::Asker asker;

    auto listStreams = asker.listStreams();

    std::cout << "nb stream : " << listStreams.size() << std::endl;
    for (const auto & [streamName, sensorSpec] : listStreams) {
        std::cout << streamName << std::endl;

//        const auto & sensorSpec = asker.getSensorSpec(streamName);
        std::cout << sensorSpec << std::endl;

        auto acq = asker.getAcquisition(streamName);
        std::cout << acq << std::endl;

        std::cout << "--------------------------------" << std::endl;
    }

//    std::this_thread::sleep_for(std::chrono::milliseconds(2000));


    return 0;
}
