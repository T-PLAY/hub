

#include <oct.h>
#include <octave-7.3.0/octave/str-vec.h>

#include <Asker.hpp>
#include <iomanip>

void usage() {
    std::cout << "usage: hub_listStream [ipv4:string] [port:int]" << std::endl;
}

DEFUN_DLD( hub_listStream, args, , "Return list of streams connected to the server" ) {

    const int n = args.length();

    octave_value_list retval = octave_value_list( std::list<std::string> { "", "", "" } );

    auto ipv4 = hub::net::s_defaultServiceIp;
    auto port = hub::net::s_defaultServicePort;

    if ( n > 2 ) {
        //        octave::print_usage();
        warning( "too many arguments" );
        usage();
    }
    else {
        bool ok = true;
        if ( n >= 1 ) {
            if ( args( 0 ).is_string() ) {
                ipv4 = args( 0 ).string_value();
                if ( !hub::net::utils::isValid( ipv4 ) ) {
                    warning( "1st argument (ipv4) must be a valid ipv4 string "
                             "[0-255].[0-255].[0-255].[0-255]" );

                    //                    usage();
                    ok = false;
                }
            }
            else {
                warning( "1st argument (ipv4) must be string" );
                //                usage();
                ok = false;
            }
        }

        if ( n >= 2 ) {

            if ( args( 1 ).is_scalar_type() ) { port = args( 1 ).int64_scalar_value(); }
            else {
                warning( "2nd argument (port) must be integer" );
                //                usage();

                ok = false;
            }
        }

        if ( ok ) {

            try {
                hub::Asker asker( ipv4, port );
                const auto listStreams = asker.listStreams();

                string_vector streamNames;
                string_vector sensorSpecs;

//                int i                      = 0;
                constexpr int marginFirst  = 40;
                constexpr int marginSecond = 0;

                std::cout << std::left << std::setw( marginFirst ) << "Stream name"
                          << std::setw( marginSecond )
                          << "| Sensor spec (sensor name, resolutions, metadata, data size)"
                          << "\033[0m" << std::endl;
                std::cout << "----------------------------------------+----------------------------"
                             "----------------------------------------------"
                          << std::endl;

                for ( const auto& [streamName, sensorSpec] : listStreams ) {
                    streamNames.append( streamName );
                    sensorSpecs.append( sensorSpec.to_string() );

                    std::cout << std::left << std::setw( marginFirst ) << streamName
                              << std::setw( marginSecond ) << "| " << sensorSpec << std::endl;
//                    ++i;
                }

                if ( !listStreams.empty() )
                    retval = octave_value_list( { streamNames, sensorSpecs } );
            }

            catch ( std::exception& ex ) {
                warning( "server not found at %s:%d ", ipv4.c_str(), port );
                usage();
            }
        }
        else { usage(); }
    }

    return retval;
}
