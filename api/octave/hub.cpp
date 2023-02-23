

// #include <octave/oct.h>
#include <octave-7.3.0/octave/oct.h>
#include <octave-7.3.0/octave/uint8NDArray.h>

#include <InputSensor.hpp>


DEFUN_DLD( hub, args, , "Return b = a^n for square matrix a, and non-negative, integral n." ) {

    hub::InputSensor inputSensor( hub::io::File(
        std::fstream( "anechoicTarget.txt", std::ios::binary | std::ios::in ) ) );

    const auto& sensorSpec = inputSensor.m_spec;
    std::cout << sensorSpec << std::endl;

    auto acq = inputSensor.getAcquisition();
    const auto & measure = acq.getMeasures().at(0);
    std::cout << measure << std::endl;
    const auto & resolution = measure.getResolution();
        //    const int n = args(1).int_value();

    const auto & dims = resolution.first;
    const auto & format = resolution.second;


    const auto & dimX = dims.at(1);
    const auto & dimY = dims.at(0);
//    int dimZ = hub::format2nByte(format);

//    const auto dimX = 10;
//    const auto dimY = 5;
//    int dimZ = hub::format2nByte(format);

    const auto * data = measure.m_data;
    const auto size = measure.m_size;

//    MArray<uint8NDArray> m(dimX, dimY);
//    Matrix m({5, 5, 3}, 3);
//    std::cout << m.byte_size() << std::endl;
//    m.byte_size() = 1;
    Matrix m(dimX, dimY);
//    uint8NDArray m(dimX);

    int cpt = 0;
    for (int i = 0; i < dimX; ++i) {
        for (int j = 0; j < dimY; ++j) {
            unsigned char y = data[cpt];
//            unsigned char g = data[cpt];
//            unsigned char r = data[cpt];
            m(i, j) = y / 255.0;
//            for (int k = 0; k < 3; ++k) {
//                m(i, j, k) = 1.0;
//            }
            assert(cpt < size);
            ++cpt;
        }
    }

        //    if (n == 0)
        //        return octave_value(
        //            DiagMatrix(args(0).rows(), args(0).columns(), 1.0)
        //            );
        //    if (n == 1)
        //        return args(0);

        //    hub::SensorSpec sensorSpec;

//        Matrix m;

//        const Matrix & m(args(0).matrix_value());


//        Matrix b(m);
//        int np = n - 1;
//        while (np >= 1)
//        {
//            if (is_even(np))
//            {
//                np = np / 2;
//            }
//            else
//            {
//                np = (np - 1) / 2;
//                b = b * p;
//            }
//            p = p * p;
//        }

//    return m;
    return octave_value( m );
    //    return octave_value(b);
}

// bool
// is_even(int n)
//{
//     return n % 2 == 0;
// }
