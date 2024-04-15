
#include <thread>

#include <sensor/outputSensor/OutputSensor_1D_Dof6.hpp>

int main(int argc, char* argv[]) {

    /** Description
Run server.
    End description */

    std::vector<std::string> args( argv + 1, argv + argc );

    int port                = HUB_SERVICE_PORT;
    std::string ipv4 = "127.0.0.1";

    auto it = args.begin();
    while ( it != args.end() ) {
        const auto& arg = *it;

        if ( arg == "-h" || arg == "--help" ) {
            std::cout << argv[0] << " usage: [--port <int>] [--ipv4 <string>]" << std::endl;
            return 0;
        }
        else if ( arg == "--port" ) {
            assert( it + 1 != args.end() );
            const auto& nextArg = *( it + 1 );
            port                = std::atoi( nextArg.c_str() );
            ++it;
        }
        else if ( arg == "--ipv4" ) {
            assert( it + 1 != args.end() );
            const auto& nextArg = *( it + 1 );
            ipv4 = nextArg;
            ++it;
        }
        else {
            std::cout << "unrecognized argument: " << arg << std::endl;
            std::cout << argv[0] << " usage: [--exitWhenServerLost]" << std::endl;
            return 0;
        }
        ++it;
    }


    hub::sensor::outputSensor::OutputSensor_1D_Dof6 outputSensorDof6(port, ipv4);
    outputSensorDof6.run();

    return 0;
}
