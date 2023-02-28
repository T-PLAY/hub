
#include <Streamer.hpp>

int main() {

    // startConstruction
    // link to local server at port 4042
    hub::Streamer streamer( "127.0.0.1", 4042 );
    // endConstruction

    hub::SensorSpec sensorSpec;

    streamer.addStream("hello", std::move(sensorSpec));


    return 0;
}
