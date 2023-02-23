
// #include "mex.hpp"
// #include "mexAdapter.hpp"
#include <MatlabDataArray.hpp> // ROW_MAJOR
#include <mex.hpp>
#include <mexAdapter.hpp>

#include <InputSensor.hpp>

// #include <array>

class MexFunction : public matlab::mex::Function
{
  public:
    void operator()( matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs ) {
        // check input arguments
        // implement function

        hub::InputSensor inputSensor( hub::io::File(
            std::fstream( "anechoicTarget.txt", std::ios::binary | std::ios::in ) ) );

        const auto& sensorSpec = inputSensor.m_spec;
        std::cout << sensorSpec << std::endl;

        auto acq            = inputSensor.getAcquisition();
        const auto& measure = acq.getMeasures().at( 0 );
        std::cout << measure << std::endl;
        const auto& resolution = measure.getResolution();
        //    const int n = args(1).int_value();

        const auto& dims   = resolution.first;
        const auto& format = resolution.second;

        const auto& dimX = dims.at( 1 );
        const auto& dimY = dims.at( 0 );
        //    int dimZ = hub::format2nByte(format);

        //    const auto dimX = 10;
        //    const auto dimY = 5;
        //    int dimZ = hub::format2nByte(format);

        const auto* data = measure.m_data;
        const auto size  = measure.m_size;

        //        using namespace matlab::data;
        matlab::data::ArrayFactory factory;
        std::vector<unsigned char> v( data, data + size );
        //        matlab::data::Array m = factory.createArray<double>({2, 2}, {1.0, 3.0, 2.0, 4.0});
        //        matlab::data::Array m = factory.createArray<unsigned char>({(unsigned long)dimX,
        //        (unsigned long)dimY}, data, data + size);
        matlab::data::Array m = factory.createArray( { (unsigned long)dimX, (unsigned long)dimY },
                                                     v.begin(),
                                                     v.end(),
                                                     matlab::data::InputLayout::ROW_MAJOR );

        //        const std::vector<unsigned char> imgData(data);

        outputs[0] = std::move( m );
        //        matlab::data::TypedArray<double> out()
        //        matlab::data::Array m()

        //    MArray<uint8NDArray> m(dimX, dimY);
        //    Matrix m({5, 5, 3}, 3);
        //    std::cout << m.byte_size() << std::endl;
        //    m.byte_size() = 1;
        //        Matrix m( dimX, dimY );
        //        //    uint8NDArray m(dimX);

        //        int cpt = 0;
        //        for ( int i = 0; i < dimX; ++i ) {
        //            for ( int j = 0; j < dimY; ++j ) {
        //                unsigned char y = data[cpt];
        //                //            unsigned char g = data[cpt];
        //                //            unsigned char r = data[cpt];
        //                m( i, j ) = y / 255.0;
        //                //            for (int k = 0; k < 3; ++k) {
        //                //                m(i, j, k) = 1.0;
        //                //            }
        //                assert( cpt < size );
        //                ++cpt;
        //            }
        //        }
    }
};
