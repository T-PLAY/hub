

#include <oct.h>
// #include <unwind-prot.h>

#include <Asker.hpp>

// void my_err_handler( const char* fmt, ... ) {
//     // Do nothing!!
// }

// void my_err_with_id_handler( const char* id, const char* fmt, ... ) {
//     // Do nothing!!
// }

void usage() {
    std::cout << "usage: hub_getAcquisition streamName:string [ipv4:string] [port:int]"
              << std::endl;
}

DEFUN_DLD( hub_getAcquisition, args, , "Return list of streams connected to the server" ) {

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

    octave_value_list retval = octave_value_list( std::list<std::string> { "", "", "", "", "", "" } );
    //    octave_value retval = octave_value( "" );


    auto ipv4 = hub::net::s_defaultServiceIp;
    auto port = hub::net::s_defaultServicePort;
    std::string streamName;

    if ( n > 3 ) {
        //        octave::print_usage();
        warning( "hub_listStream: too many arguments" );
        usage();
    }
    else if ( n < 1 ) {
        //        octave::print_usage();
        warning( "hub_listStream: need at least one argument" );
        usage();
    }
    else {
        bool ok = true;
        if ( n >= 1 ) {

            if ( args( 0 ).is_string() ) { streamName = args( 0 ).string_value(); }
            else {
                warning( "hub_listStream: 1st argument (streamName) must be string" );
                //                usage();

                ok = false;
            }
        }

        if ( n >= 2 ) {
            if ( args( 1 ).is_string() ) { ipv4 = args( 1 ).string_value(); }
            else {
                warning( "hub_listStream: 2nd argument (ipv4) must be string" );
                if ( !hub::net::utils::isValid( ipv4 ) ) {
                    warning( "hub_listStream: 2nd argument (ipv4) must be a truly ipv4 string "
                             "0-255.0-255.0-255.0-255" );
                    //                    usage();
                    ok = false;
                }
                //                usage();
                ok = false;
            }
        }

        if ( n >= 3 ) {

            if ( args( 2 ).is_scalar_type() ) { port = args( 2 ).int64_scalar_value(); }
            else {
                warning( "hub_listStream: 3rd argument (port) must be integer" );
                //                usage();

                ok = false;
            }
        }

        if ( ok ) {

            try {
                hub::Asker asker( ipv4, port );

                auto acq = asker.getAcquisition( streamName );
                //                const auto listStreams = asker.listStreams();
                //    auto acq = inputSensor.getAcquisition();
                const auto& measure = acq.getMeasures().at( 0 );
                std::cout << measure << std::endl;
                const auto& resolution = measure.getResolution();
                //    const int n = args(1).int_value();

                const auto& dims   = resolution.first;
                const auto& format = resolution.second;

                auto dimX = 1;
                auto dimY = 1;

                if ( dims.size() == 1 ) { dimY = dims.at( 0 ); }
                else if ( dims.size() == 2 ) {
                    dimX = dims.at( 1 );
                    dimY = dims.at( 0 );
                }
                else
                    assert( false );

                //    int dimZ = hub::format2nByte(format);

                //    const auto dimX = 10;
                //    const auto dimY = 5;
                //    int dimZ = hub::format2nByte(format);

                const auto* data = measure.m_data;
                const auto size  = measure.m_size;

                if ( format == hub::Format::DOF6 ) {
                    Matrix pos( 1, 3 );
                    Matrix quat( 1, 4 );
                    //    uint8NDArray m(dimX);

                    //                    m(1, 1) = 0;
                    //                    m(1, 0) = 0;
                    const hub::Dof6& dof6 = measure;
                    std::cout << "dof6: " << dof6 << std::endl;

                    pos( 0, 0 ) = dof6.x();
                    pos( 0, 1 ) = dof6.y();
                    pos( 0, 2 ) = dof6.z();
                    quat( 0, 0 ) = dof6.w0();
                    quat( 0, 1 ) = dof6.w1();
                    quat( 0, 2 ) = dof6.w2();
                    quat( 0, 3 ) = dof6.w3();

//                    retval = octave_value( m );
                    retval = octave_value_list( { acq.m_start, acq.m_end, 2, pos, quat, ""} );
                }
                else {


                    //    MArray<uint8NDArray> m(dimX, dimY);
                    //    Matrix m({5, 5, 3}, 3);
                    //    std::cout << m.byte_size() << std::endl;
                    //    m.byte_size() = 1;
                    //                    Matrix m( dimX, dimY );
                    //    uint8NDArray m(dimX);

                    //				    retval = octave_value_list( std::list<std::string> { "" } );
                    Matrix channel( dimX, dimY );
                    Matrix channel2( dimX, dimY );
                    Matrix channel3( dimX, dimY );

                    int nChannels = 0;
                    switch ( format ) {
                    case hub::Format::Y8:
                        nChannels = 1;
                        break;
                    case hub::Format::Y16:
                        nChannels = 2;
                        break;
                    case hub::Format::BGR8:
                    case hub::Format::RGB8:
                        nChannels = 3;
                        break;
                    case hub::Format::RGBA8:
                        nChannels = 4;
                        break;
                    default:
                        assert( false );
                        break;
                    }

                    //                    int cpt = 0;
                    for ( int i = 0; i < dimX; ++i ) {
                        for ( int j = 0; j < dimY; ++j ) {
                            channel( i, j ) = data[( i * dimY + j ) * nChannels] / 255.0;
                            if ( nChannels >= 2 )
                                channel2( i, j ) = data[( i * dimY + j ) * nChannels + 1] / 255.0;
                            if ( nChannels >= 3 )
                                channel3( i, j ) = data[( i * dimY + j ) * nChannels + 2] / 255.0;

                            //                            int sum = 0;
                            //                            for ( int k = 0; k < nChannels; ++k ) {
                            //                                unsigned char y = data[cpt];
                            //            unsigned char g = data[cpt];
                            //            unsigned char r = data[cpt];
                            //                                sum += data[(i * dimY + j) * nChannels
                            //                                + k];
                            //                            }
                            //                            m( i, j ) = sum / (nChannels * 255.0);

                            //            for (int k = 0; k < 3; ++k) {
                            //                m(i, j, k) = 1.0;
                            //            }
                            //                                assert( cpt < size );
                            //                                ++cpt;
                        }
                    }
                    //                    retval = octave_value( m );
                    if ( nChannels == 1 )
//                        retval = octave_value_list( octave_llist<Matrix>{ channel });
                        retval = octave_value_list( { acq.m_start, acq.m_end, nChannels, channel, "", "" } );
                    if ( nChannels == 2 )
//                        retval = octave_value_list( { channel, channel2});
                        retval = octave_value_list( { acq.m_start, acq.m_end, nChannels, channel, channel2, "" } );
                    if ( nChannels == 3 )
                        retval = octave_value_list( { acq.m_start, acq.m_end, nChannels, channel, channel2, channel3 } );
                }


                //                if ( !listStreams.empty() ) retval = octave_value_list(
                //                listStreams );
            }

            catch ( std::exception& ex ) {
                //                warning( "hub_listStream: server not found at %s:%d ",
                //                ipv4.c_str(), port );
                warning( "hub_listStream: %s", ex.what() );
                usage();
            }
        }
        else { usage(); }
    }

    return retval;
}
