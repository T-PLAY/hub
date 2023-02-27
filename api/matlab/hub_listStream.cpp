
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
    std::cout << "usage: hub_listStream [ipv4:string] [port:int]" << std::endl;
}

//#define warning(args) printf(args)

void warning(const std::string & message) {
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
        outputs[0] = factory.createEmptyArray();
        outputs[1] = factory.createEmptyArray();
//        matlab::data::Array m = factory.createArray( { (unsigned long)dimX, (unsigned long)dimY },
//                                                     v.begin(),
//                                                     v.end(),
//                                                     matlab::data::InputLayout::ROW_MAJOR );

        //        const std::vector<unsigned char> imgData(data);

//        outputs[0] = matlab::data::StringArray({std::string("hello")});
//        outputs[0] = std::move(matlab::data::nullopt(0));

//        octave_value_list retval = octave_value_list( std::list<std::string> { "", "", "" } );
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
                if ( inputs[0].getType() == matlab::data::ArrayType::MATLAB_STRING ) {
//                if ( inputs( 0 ).is_string() ) {
//                    ipv4 = args( 0 ).string_value();
                    ipv4 = std::string(inputs[0][0]);
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

//                if ( args( 1 ).is_scalar_type() ) { port = args( 1 ).int64_scalar_value(); }
//                std::cout << "input 1 " << std::to_string((int)inputs[1].getType()) << std::endl;
                if ( inputs[1].getType() == matlab::data::ArrayType::DOUBLE ) { port = (int)inputs[ 1 ][0]; }
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

                    auto streamNames = factory.createArray<matlab::data::MATLABString>({1, listStreams.size()});
                    auto sensorSpecs = factory.createArray<matlab::data::MATLABString>({1, listStreams.size()});
//                    string_vector streamNames;
//                    string_vector sensorSpecs;

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


                    int i = 0;
                    for ( const auto& [streamName, sensorSpec] : listStreams ) {
//                        streamNames.append( streamName );
                        streamNames[i] = streamName;
//                        sensorSpecs.append( sensorSpec.to_string() );
                        sensorSpecs[i] = sensorSpec.to_string();

                        std::cout << std::left << std::setw( marginFirst ) << streamName
                                  << std::setw( marginSecond ) << "| " << sensorSpec << std::endl;
                                            ++i;
                    }


//                    if ( !listStreams.empty() )
//                        retval = octave_value_list( { streamNames, sensorSpecs } );

                    outputs[0] = streamNames;
                    outputs[1] = sensorSpecs;
                }

                catch ( std::exception& ex ) {
                    warning( std::string("server not found at ") + ipv4 + ":" + std::to_string(port) );
                    usage();
                }
            }
            else { usage(); }
        }



    }
};
