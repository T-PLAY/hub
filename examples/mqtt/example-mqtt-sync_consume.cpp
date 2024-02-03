

#include "mqtt/client.h"
#include <cctype>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>

using namespace std;
using namespace std::chrono;

const string SERVER_ADDRESS { "tcp://localhost:1883" };
const string CLIENT_ID { "paho_cpp_sync_consume" };
int main( int argc, char* argv[] ) {
    mqtt::client cli( SERVER_ADDRESS, CLIENT_ID );

    auto connOpts = mqtt::connect_options_builder()
                        .user_name( "user" )
                        .password( "passwd" )
                        .keep_alive_interval( seconds( 30 ) )
                        .automatic_reconnect( seconds( 2 ), seconds( 30 ) )
                        .clean_session( false )
                        .finalize();

    // You can install a callback to change some connection data
    cli.set_update_connection_handler( []( mqtt::connect_data& connData ) {
        string newUserName { "newuser" };
        if ( connData.get_user_name() == newUserName ) return false;

        cout << "Previous user: '" << connData.get_user_name() << "'" << endl;
        connData.set_user_name( newUserName );
        cout << "New user name: '" << connData.get_user_name() << "'" << endl;
        return true;
    } );

    const vector<string> TOPICS { "data/#", "command" };
    const vector<int> QOS { 0, 1 };

    try {
        cout << "Connecting to the MQTT server..." << flush;
        mqtt::connect_response rsp = cli.connect( connOpts );
        cout << "OK\n" << endl;

        if ( !rsp.is_session_present() ) {
            std::cout << "Subscribing to topics..." << std::flush;
            cli.subscribe( TOPICS, QOS );
            std::cout << "OK" << std::endl;
        }
        else {
            cout << "Session already present. Skipping subscribe." << std::endl;
        }

        // Consume messages

        while ( true ) {
            auto msg = cli.consume_message();

            if ( msg ) {
                if ( msg->get_topic() == "command" && msg->to_string() == "exit" ) {
                    cout << "Exit command received" << endl;
                    break;
                }

                cout << msg->get_topic() << ": " << msg->to_string() << endl;
            }
            else if ( !cli.is_connected() ) {
                cout << "Lost connection" << endl;
                while ( !cli.is_connected() ) {
                    this_thread::sleep_for( milliseconds( 250 ) );
                }
                cout << "Re-established connection" << endl;
            }
        }

        // Disconnect

        cout << "\nDisconnecting from the MQTT server..." << flush;
        cli.disconnect();
        cout << "OK" << endl;
    }
    catch ( const mqtt::exception& exc ) {
        cerr << exc.what() << endl;
        return 1;
    }

    return 0;
}

//// Start by `#include`-ing the Mosquitto MQTT Library and other standard libraries.

//// With the library header files included, continue by defining a main function.

////    mqtt::client client(ip, id, mqtt::create_options(MQTTVERSION_DEFAULT));
