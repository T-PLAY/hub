
#include <stream.h>
#include <constants.h>

int main(int argc, char* argv[])
{

    Stream::MetaData metaData;
    metaData["scanWidth"] = 20.0;
    metaData["scanDepth"] = 20.0;
    metaData["x"] = 0.0;
    metaData["y"] = 0.0;
    metaData["z"] = 0.0;
    metaData["mriFile"] = "AXT2_ligaments_uterosacres/D0010525.dcm";

    InputStream inputPosStream(g_probePoseSensorName);

    OutputStream outputPosStream(g_probePoseSensorName + " (simulator)", inputPosStream.getFormat(), inputPosStream.getDims(), ClientSocket(), metaData);


    while (true) {
        const auto & acq = inputPosStream.getAcquisition();
        outputPosStream << acq;
    }

    return 0;
}
