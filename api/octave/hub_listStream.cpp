

#include <oct.h>
// #include <unwind-prot.h>
#include <octave-7.3.0/octave/str-vec.h>

#include <Asker.hpp>
#include <iomanip>


// void my_err_handler( const char* fmt, ... ) {
//     // Do nothing!!
// }

// void my_err_with_id_handler( const char* id, const char* fmt, ... ) {
//     // Do nothing!!
// }

void usage() {
    std::cout << "usage: hub_listStream [ipv4:string] [port:int]" << std::endl;
}

DEFUN_DLD( hub_listStream, args, , "Return list of streams connected to the server" ) {

    const int n = args.length();

    // Declare unwind_protect frame which lasts as long as
    // the variable frame has scope.
    //    unwind_protect frame;
    //    octave::unwind_protect frame;
    //    frame.add_fcn( set_liboctave_warning_handler, current_liboctave_warning_handler );

    //    frame.add_fcn( set_liboctave_warning_with_id_handler,
    //                   current_liboctave_warning_with_id_handler );

    //    set_liboctave_warning_handler( my_err_handler );
    //    set_liboctave_warning_with_id_handler( my_err_with_id_handler );

//    octave_value_list retval = octave_value_list( std::list<std::string> { "" } );
    octave_value_list retval = octave_value_list( std::list<std::string> { "", "", "" } );

    auto ipv4 = hub::net::s_defaultServiceIp;
    auto port = hub::net::s_defaultServicePort;

    //    dim_vector dim({2, 1});
    //    MArray<std::string> streamNames({2}, std::string(""));
    //    charNDArray streamNames(2);
    //    Array<std::string> streamNames();
    //    streamNames.fill("hello");

    if ( n > 2 ) {
        //        octave::print_usage();
        warning( "hub_listStream: too many arguments" );
        usage();
    }
    else {
        bool ok = true;
        if ( n >= 1 ) {
            if ( args( 0 ).is_string() ) {
                ipv4 = args( 0 ).string_value();
                if ( !hub::net::utils::isValid( ipv4 ) ) {
                    warning( "hub_listStream: 1st argument (ipv4) must be a truly ipv4 string "
                             "0-255.0-255.0-255.0-255" );
                    //                    usage();
                    ok = false;
                }
            }
            else {
                warning( "hub_listStream: 1st argument (ipv4) must be string" );
                //                usage();
                ok = false;
            }
        }

        if ( n >= 2 ) {

            if ( args( 1 ).is_scalar_type() ) { port = args( 1 ).int64_scalar_value(); }
            else {
                warning( "hub_listStream: 2nd argument (port) must be integer" );
                //                usage();

                ok = false;
            }
        }

        if ( ok ) {


            try {
                hub::Asker asker( ipv4, port );
                const auto listStreams = asker.listStreams();

//                std::cout << "nb el = " << listStreams.size() << std::endl;


                //                Array<std::string> streamNames( { (int)listStreams.size(), 1 } );
//                Array<Array<char>> streamNames( { (int)listStreams.size(), 1 } );
                string_vector streamNames;
                string_vector sensorSpecs;
//                NDArray streamNames( { (int)listStreams.size(), 1 } );
                //                NDArray


                //                Array<std::string> streamNames({(int)listStreams.size(), 1},
                //                std::cat(listStreams.begin(), listStreams.end()));
                //				return octave_value(streamNames);
                int i = 0;
                constexpr int marginFirst = 40;
                constexpr int marginSecond = 0;

                std::cout << std::left << std::setw( marginFirst ) << "Stream name" << std::setw( marginSecond ) << "| Sensor spec (sensor name, resolutions, metadata, data size)" << "\033[0m" << std::endl;
                std::cout << "----------------------------------------+--------------------------------------------------------------------------" << std::endl;
                for ( const auto& [streamName, sensorSpec] : listStreams ) {
                    streamNames.append(streamName);
                    sensorSpecs.append(sensorSpec.to_string());

//                    std::cout << "streamName: " << streamName << ", sensorSpec: " << sensorSpec << std::endl;
                    std::cout << std::left << std::setw(marginFirst) << streamName << std::setw(marginSecond) << "| " << sensorSpec << std::endl;
//                    streamNames(i) = streamName;
//                    for ( int j = 0; j < streamName.size(); ++j ) {



//                        streamNames( i, j ) = streamName.at( j );
//                    }
//                    streamNames(i, streamName.size()) = '\0';
                    ++i;
                }

                //                }
                //                if ( !listStreams.empty() ) retval = octave_value_list(
                //                listStreams );
                if ( !listStreams.empty() )
                    retval = octave_value_list( {streamNames, sensorSpecs} );

                //                std::cout << "nb el = " << retval.length() << std::endl;
            }
            catch ( std::exception& ex ) {
                warning( "hub_listStream: server not found at %s:%d ", ipv4.c_str(), port );
                usage();
            }
        }
        else { usage(); }
    }

    return retval;
}
