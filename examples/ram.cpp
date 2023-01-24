
#include <IO/Ram.hpp>
#include <InputSensor.hpp>
#include <OutputSensor.hpp>



///
/// \file
/// \brief  Main function
/// describes how use a RAM to init input/output sensor stream.
///

int main() {

    {
        // clang-format off
        // startConstruction
hub::io::CyclicBuff buff;
hub::io::Ram inputRam(buff);
hub::io::Ram outputRam(buff);
        // endConstruction
        // clang-format on
    }


    return 0;
}
