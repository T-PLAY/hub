

#include <thread>

#include <client/Asker.hpp>

/// \file
/// hello
/// @file
/// nice
/// \warning
/// Needs running server.
/// \brief  Main function
/// describes how use a file to init input/output sensor stream
///
int main() {

    hub::client::Asker asker;

    auto listStreams = asker.listStreams();

    std::cout << "nb stream : " << listStreams.size() << std::endl;
#if ( __cplusplus >= 201703L )
    for ( const auto& [streamName, sensorSpec] : listStreams ) {
#else
    for ( const auto& pair : listStreams ) {
        const auto& streamName = pair.first;
        const auto& sensorSpec = pair.second;
#endif
        std::cout << streamName << std::endl;

        std::cout << sensorSpec << std::endl;

        auto acq = asker.getAcquisition( streamName );
        std::cout << acq << std::endl;

        std::cout << "--------------------------------" << std::endl;
    }

    return 0;
}
