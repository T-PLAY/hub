
// #include "mex.hpp"
// #include "mexAdapter.hpp"
#include <iomanip>

#include <MatlabDataArray.hpp> // ROW_MAJOR
#undef DLL_EXPORT_SYM
#include <mex.hpp>
#include <mexAdapter.hpp>

#include <Asker.hpp>

// #include <array>

void usage() {
    std::cout << "usage: hub_getAcquisition streamName:string [ipv4:string] [port:int]" << std::endl;
}

// #define warning(args) printf(args)

void warning( const std::string& message ) {
    std::cout << "warning: " << message << std::endl;
}

#include <MatlabDataArray/String.hpp>

class MexFunction : public matlab::mex::Function
{
  public:
    void operator()( matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs ) {
        // check input arguments
        // implement function

        const int n = inputs.size();

        matlab::data::ArrayFactory factory;
        for ( int i = 0; i < 7; ++i )
            outputs[i] = factory.createEmptyArray();

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

                //                if ( args( 0 ).is_string() ) { streamName = args( 0
                //                ).string_value(); }
                if ( inputs[0].getType() == matlab::data::ArrayType::MATLAB_STRING ) {
                    streamName = std::string( inputs[0][0] );
                }
                else {
                    warning( "1st argument (streamName) must be string" );

                    ok = false;
                }
            }

            if ( n >= 2 ) {
                //                if ( inputs[ 1 ].is_string() ) { ipv4 = inputs[ 1
                //                ].string_value(); }
                if ( inputs[1].getType() == matlab::data::ArrayType::MATLAB_STRING ) {
                    ipv4 = std::string( inputs[1][0] );
                }
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

                //                if ( inputs[ 2 ].is_scalar_type() ) { port = inputs[ 2
                //                ].int64_scalar_value(); }
                if ( inputs[2].getType() == matlab::data::ArrayType::DOUBLE ) {
                    port = (int)inputs[2][0];
                }
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
                        //                        Matrix pos( 1, 3 );
                        //                        auto pos =
                        //                        factory.createArray<matlab::data::ArrayType::DOUBLE>({1,
                        //                        3});
                        matlab::data::Array pos = factory.createArray( { 1, 3 }, data, data + 3 );
                        matlab::data::Array quat =
                            factory.createArray( { 1, 4 }, data + 3, data + 7 );
                        //                        factory.createArrayFromBuffer({1, 3},
                        //                        Matrix quat( 1, 4 );

                        const hub::Dof6& dof6 = measure;
                        std::cout << "dof6: " << dof6 << std::endl;

                        //                        pos( 0, 0 )  = dof6.x();
                        //                        pos( 0, 1 )  = dof6.y();
                        //                        pos( 0, 2 )  = dof6.z();
                        //                        quat( 0, 0 ) = dof6.w0();
                        //                        quat( 0, 1 ) = dof6.w1();
                        //                        quat( 0, 2 ) = dof6.w2();
                        //                        quat( 0, 3 ) = dof6.w3();


                        outputs[0] = pos;
                        outputs[1] = quat;

                        outputs[3] = factory.createArray<int>({1}, {2});
                        //                        retval = octave_value_list(
                        //                            { pos, quat, "", 2, hub::to_string( format ),
                        //                            acq.m_start, acq.m_end } );
                    }
                    else {

                        //                        Matrix channel( dimX, dimY );
                        matlab::data::Array channel = factory.createArray<double>(
                            { (unsigned long)dimX, (unsigned long)dimY } );
                        matlab::data::Array channel2 = factory.createArray<double>(
                            { (unsigned long)dimX, (unsigned long)dimY } );
                        matlab::data::Array channel3 = factory.createArray<double>(
                            { (unsigned long)dimX, (unsigned long)dimY } );
                        //                                                     v.begin(),
                        //                                                     v.end(),
                        //                                                     matlab::data::InputLayout::ROW_MAJOR
                        //                                                     );
                        //                        Matrix channel2( dimX, dimY );
                        //                        Matrix channel3( dimX, dimY );

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
                                channel[i][j] = data[( i * dimY + j ) * nChannels] / 255.0;
                                if ( nChannels >= 2 )
                                    channel2[i][j] = data[( i * dimY + j ) * nChannels + 1] / 255.0;
                                if ( nChannels >= 3 )
                                    channel3[i][j] = data[( i * dimY + j ) * nChannels + 2] / 255.0;
                            }
                        }
                        //                        if ( nChannels == 1 )
                        outputs[0] = channel;
                        outputs[1] = channel2;
                        outputs[2] = channel3;

                        outputs[3] = factory.createArray<int>({1}, {nChannels});

//                        outputs[5] = acq.m_start;
//                        outputs[6] = acq.m_end;
                        //                            retval = octave_value_list( { channel,
                        //                                                          "",
                        //                                                          "",
                        //                                                          nChannels,
                        //                                                          hub::to_string(
                        //                                                          format ),
                        //                                                          acq.m_start,
                        //                                                          acq.m_end } );
                        //                        if ( nChannels == 2 )
                        //                            retval = octave_value_list( { channel,
                        //                                                          channel2,
                        //                                                          "",
                        //                                                          nChannels,
                        //                                                          hub::to_string(
                        //                                                          format ),
                        //                                                          acq.m_start,
                        //                                                          acq.m_end } );
                        //                        if ( nChannels == 3 )
                        //                            retval = octave_value_list( { channel,
                        //                                                          channel2,
                        //                                                          channel3,
                        //                                                          nChannels,
                        //                                                          hub::to_string(
                        //                                                          format ),
                        //                                                          acq.m_start,
                        //                                                          acq.m_end } );
                    }

                        //                            outputs[4] = hub::to_string(format);
                        auto formats = factory.createArray<matlab::data::MATLABString>( { 1, 1 } );
                        formats[0][0]  = hub::to_string( format );
                        outputs[4]   = formats;

                        outputs[5] = factory.createArray<long>({1, 1}, {acq.m_start});
                        outputs[6] = factory.createArray<long>({1, 1}, {acq.m_end});
                }

                catch ( std::exception& ex ) {
                    warning( ex.what() );
                    usage();
                }
            }
            else { usage(); }
        }
    }
};
