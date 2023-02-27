

#include <oct.h>

#include <Asker.hpp>

void usage() {
    std::cout << "usage: hub_getAcquisition streamName:string [ipv4:string] [port:int]"
              << std::endl;
}

DEFUN_DLD( hub_getAcquisition, args, , "Return list of streams connected to the server" ) {

    const int n = args.length();

    octave_value_list retval =
        octave_value_list( std::list<std::string> { "", "", "", "", "", "", "" } );

    auto ipv4 = hub::net::s_defaultServiceIp;
    auto port = hub::net::s_defaultServicePort;
    std::string streamName;

    if ( n > 3 ) {
        warning( "too many arguments" );
        usage();
    }
    else if ( n < 1 ) {
        warning( "need at least one argument" );
        usage();
    }
    else {
        bool ok = true;
        if ( n >= 1 ) {

            if ( args( 0 ).is_string() ) { streamName = args( 0 ).string_value(); }
            else {
                warning( "1st argument (streamName) must be string" );

                ok = false;
            }
        }

        if ( n >= 2 ) {
            if ( args( 1 ).is_string() ) { ipv4 = args( 1 ).string_value(); }
            else {
                warning( "2nd argument (ipv4) must be string" );
                if ( !hub::net::utils::isValid( ipv4 ) ) {
                    warning( "2nd argument (ipv4) must be a truly ipv4 string "
                             "0-255.0-255.0-255.0-255" );
                    ok = false;
                }
                //                usage();
                ok = false;
            }
        }

        if ( n >= 3 ) {

            if ( args( 2 ).is_scalar_type() ) { port = args( 2 ).int64_scalar_value(); }
            else {
                warning( "3rd argument (port) must be integer" );

                ok = false;
            }
        }

        if ( ok ) {

            try {
                hub::Asker asker( ipv4, port );

                auto acq            = asker.getAcquisition( streamName );
                const auto& measure = acq.getMeasures().at( 0 );
                std::cout << measure << std::endl;
                const auto& resolution = measure.getResolution();

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

                const auto* data = measure.m_data;
                const auto size  = measure.m_size;

                if ( format == hub::Format::DOF6 ) {
                    Matrix pos( 1, 3 );
                    Matrix quat( 1, 4 );

                    const hub::Dof6& dof6 = measure;
                    std::cout << "dof6: " << dof6 << std::endl;

                    pos( 0, 0 )  = dof6.x();
                    pos( 0, 1 )  = dof6.y();
                    pos( 0, 2 )  = dof6.z();
                    quat( 0, 0 ) = dof6.w0();
                    quat( 0, 1 ) = dof6.w1();
                    quat( 0, 2 ) = dof6.w2();
                    quat( 0, 3 ) = dof6.w3();

                    retval = octave_value_list(
                        { pos, quat, "", 2, hub::to_string( format ), acq.m_start, acq.m_end } );
                }
                else {

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

                    for ( int i = 0; i < dimX; ++i ) {
                        for ( int j = 0; j < dimY; ++j ) {
                            channel( i, j ) = data[( i * dimY + j ) * nChannels] / 255.0;
                            if ( nChannels >= 2 )
                                channel2( i, j ) = data[( i * dimY + j ) * nChannels + 1] / 255.0;
                            if ( nChannels >= 3 )
                                channel3( i, j ) = data[( i * dimY + j ) * nChannels + 2] / 255.0;
                        }
                    }
                    if ( nChannels == 1 )
                        retval = octave_value_list( { channel,
                                                      "",
                                                      "",
                                                      nChannels,
                                                      hub::to_string( format ),
                                                      acq.m_start,
                                                      acq.m_end } );
                    if ( nChannels == 2 )
                        retval = octave_value_list( { channel,
                                                      channel2,
                                                      "",
                                                      nChannels,
                                                      hub::to_string( format ),
                                                      acq.m_start,
                                                      acq.m_end } );
                    if ( nChannels == 3 )
                        retval = octave_value_list( { channel,
                                                      channel2,
                                                      channel3,
                                                      nChannels,
                                                      hub::to_string( format ),
                                                      acq.m_start,
                                                      acq.m_end } );
                }
            }

            catch ( std::exception& ex ) {
                warning( "%s", ex.what() );
                usage();
            }
        }
        else { usage(); }
    }

    return retval;
}
