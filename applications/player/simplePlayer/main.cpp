
//#include <cassert>
//#include <filesystem>
//#include <iostream>
//#include <thread>

//#include <Player.h>

//#include <InputSensor.hpp>
//#include <OutputSensor.hpp>

//#include <IO/File.hpp>
//#include <IO/Stream.hpp>

#include <thread>

//#include <filesystem>
//#include <set>
#include <Configurations.hpp>
#include <Loader.h>

int main( int argc, char* argv[] ) {
    std::string recordPath = PROJECT_DIR "data/records/latest/";

    Loader loader;
    loader.load( {recordPath} );

    while ( true ) {
        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    }

    return 0;
}

// struct Snap {

//};

// int main(int argc, char* argv[])
//{

/// hub::io::OutputStream("Player (" + streamName + ")"));
/// streamName + ")"));

//}
