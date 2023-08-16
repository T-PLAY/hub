

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cctype>
#include <thread>
#include <chrono>
#include "mqtt/client.h"

using namespace std;
using namespace std::chrono;

const string SERVER_ADDRESS	{ "tcp://localhost:1883" };
const string CLIENT_ID		{ "paho_cpp_sync_consume" };
int main(int argc, char* argv[])
{
    mqtt::client cli(SERVER_ADDRESS, CLIENT_ID);

    auto connOpts = mqtt::connect_options_builder()
                        .user_name("user")
                        .password("passwd")
                        .keep_alive_interval(seconds(30))
                        .automatic_reconnect(seconds(2), seconds(30))
                        .clean_session(false)
                        .finalize();

           // You can install a callback to change some connection data
           // on auto reconnect attempts. To make a change, update the
           // `connect_data` and return 'true'.
    cli.set_update_connection_handler(
        [](mqtt::connect_data& connData) {
            string newUserName { "newuser" };
            if (connData.get_user_name() == newUserName)
                return false;

            cout << "Previous user: '" << connData.get_user_name()
                 << "'" << endl;
            connData.set_user_name(newUserName);
            cout << "New user name: '" << connData.get_user_name()
                 << "'" << endl;
            return true;
        }
        );

    const vector<string> TOPICS { "data/#", "command" };
    const vector<int> QOS { 0, 1 };

    try {
        cout << "Connecting to the MQTT server..." << flush;
        mqtt::connect_response rsp = cli.connect(connOpts);
        cout << "OK\n" << endl;

        if (!rsp.is_session_present()) {
            std::cout << "Subscribing to topics..." << std::flush;
            cli.subscribe(TOPICS, QOS);
            std::cout << "OK" << std::endl;
        }
        else {
            cout << "Session already present. Skipping subscribe." << std::endl;
        }

               // Consume messages

        while (true) {
            auto msg = cli.consume_message();

            if (msg) {
                if (msg->get_topic() == "command" &&
                     msg->to_string() == "exit") {
                    cout << "Exit command received" << endl;
                    break;
                }

                cout << msg->get_topic() << ": " << msg->to_string() << endl;
            }
            else if (!cli.is_connected()) {
                cout << "Lost connection" << endl;
                while (!cli.is_connected()) {
                    this_thread::sleep_for(milliseconds(250));
                }
                cout << "Re-established connection" << endl;
            }
        }

               // Disconnect

        cout << "\nDisconnecting from the MQTT server..." << flush;
        cli.disconnect();
        cout << "OK" << endl;
    }
    catch (const mqtt::exception& exc) {
        cerr << exc.what() << endl;
        return 1;
    }

    return 0;
}

//// Start by `#include`-ing the Mosquitto MQTT Library and other standard libraries.
//#include <mqtt/client.h>  // Mosquitto client.
//#include <ostream>  // std::cout.

//// With the library header files included, continue by defining a main function.
//int main()
//{
//    // In order to connect the mqtt client to a broker,
//    // Define an Ip address pointing to a broker. In this case, the localhost on port 1883.
//    std::string ip = "localhost:1883";
//    // Then, define an ID to be used by the client when communicating with the broker.
//    std::string id = "consumer";

//           // Construct a client using the Ip and Id, specifying usage of MQTT V5.
//    mqtt::client client(ip, id, mqtt::create_options(MQTTVERSION_5));
////    mqtt::client client(ip, id, mqtt::create_options(MQTTVERSION_DEFAULT));
//    // Use the connect method of the client to establish a connection to the broker.
//    client.connect();
//    // In order to receive messages from the broker, specify a topic to subscribe to.
//    client.subscribe("in");
//    // Begin the client's message processing loop, filling a queue with messages.
//    client.start_consuming();

//    bool running = true;
//    while (running)
//    {
//        // Construct a message pointer to hold an incoming message.
//        mqtt::const_message_ptr messagePointer;

//               // Try to consume a message, passing messagePointer by reference.
//               // If a message is consumed, the function will return `true`,
//               // allowing control to enter the if-statement body.
//        if (client.try_consume_message(&messagePointer))
//        {
//            // Construct a string from the message payload.
//            std::string messageString = messagePointer -> get_payload_str();
//            // Print payload string to console (debugging).
//            std::cout << messageString << std::endl;

//                   // Perform processing on the string.
//                   // This is where message processing can be passed onto different
//                   // functions for parsing.
//                   // Here, we break the loop and exit the program if a `quit` is received.
//            if (messageString == "quit")
//            {
//                running = false;
//            }
//        }
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
//    }

//    return 0;
//}
