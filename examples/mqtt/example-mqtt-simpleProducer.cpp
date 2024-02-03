

// Start by `#include`-ing the Mosquitto MQTT Library and other standard libraries.
#include <chrono>        // Time keeping.
#include <mqtt/client.h> // Mosquitto client.
#include <ostream>       // std::cout.
#include <thread>        // Sleep.

// With the library header files included, continue by defining a main function.
int main() {
    // In order to connect the mqtt client to a broker,
    // Define an Ip address pointing to a broker. In this case, the localhost on port 1883.
    std::string ip = "localhost:1883";
    // Then, define an ID to be used by the client when communicating with the broker.
    std::string id = "publisher";

    // Construct a client using the Ip and Id, specifying usage of MQTT V5.
    mqtt::client client( ip, id, mqtt::create_options( MQTTVERSION_5 ) );
    // Use the connect method of the client to establish a connection to the broker.
    client.connect();
    // Initialize an empty message with specified topic.
    mqtt::message_ptr timeLeftMessagePointer = mqtt::make_message( "sensor", "" );

    // Create a loop for counting down from N seconds.
    for ( int i = 10; i > 0; --i ) {
        // Sleep for one second.
        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
        // Configure Mqtt message to contain payload specifying time until end.
        timeLeftMessagePointer->set_payload( "Time Left: " + std::to_string( i ) );
        // Publish the Mqtt message using the connected client.
        client.publish( timeLeftMessagePointer );
        std::cout << "send message " << i << std::endl;
    }

    // After counting down, configure Mqtt message for sending the quit signal.
    timeLeftMessagePointer->set_payload( "quit" );
    // Send quit signal to listeners.
    client.publish( timeLeftMessagePointer );

    return 0;
}
