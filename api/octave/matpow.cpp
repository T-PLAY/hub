


//#include <octave/oct.h>
#include <octave-7.3.0/octave/oct.h>

#include <Viewer.hpp>

static bool is_even(int n);


DEFUN_DLD(matpow, args, ,
           "Return b = a^n for square matrix a, and non-negative, integral n.")
{
    const int n = args(1).int_value();

    if (n == 0)
        return octave_value(
            DiagMatrix(args(0).rows(), args(0).columns(), 1.0)
            );
    if (n == 1)
        return args(0);


    hub::SensorSpec sensorSpec;

    Matrix p(args(0).matrix_value());
    Matrix b(p);
    int np = n - 1;
    while (np >= 1)
    {
        if (is_even(np))
        {
            np = np / 2;
        }
        else
        {
            np = (np - 1) / 2;
            b = b * p;
        }
        p = p * p;
    }

    return octave_value(b);
}


bool
is_even(int n)
{
    return n % 2 == 0;
}
