
#include <constants.h>
#include <stream.h>

#include <Scus.h>
#include <glm/gtc/type_ptr.hpp>

int main(int argc, char* argv[])
{

    Stream::MetaData metaData;
    metaData["scanWidth"] = 256.0;
    metaData["scanDepth"] = 256.0;
    metaData["x"] = 0.0;
    metaData["y"] = 0.0;
    metaData["z"] = 0.0;
    metaData["mriFile"] = "AXT2_ligaments_uterosacres/D0010525.dcm";

    InputStream inputPosStream(g_probePoseSensorName);
    OutputStream outputPosStream(g_probePoseSensorName + " (simulator)", inputPosStream.getFormat(), inputPosStream.getDims(), ClientSocket(), metaData);

//    const int width = 192;
//    const int height = 512;
    const int width = 256;
    const int height = 256;
    OutputStream outputScanStream(g_probeScanSensorName + " (simulator)", Stream::Format::Y8, { width, height }, ClientSocket());

    std::string filename = MRI_PATH "AXT2_ligaments_uterosacres/D0010525.dcm";

    if (!std::filesystem::exists(filename)) {
        std::cout << "file '" << filename << "' doesn't exist" << std::endl;
        exit(1);
    }
    assert(std::filesystem::exists(filename));
    auto filepath = std::filesystem::path(filename).remove_filename();

    std::vector<std::filesystem::path> files_in_directory;
    for (const auto& entry : std::filesystem::directory_iterator(filepath.remove_filename())) {
        if (!std::filesystem::is_directory(entry)) {
            files_in_directory.push_back(entry);
        }
    }
    std::sort(files_in_directory.begin(), files_in_directory.end());

    std::vector<std::string> fileList;
    for (const auto& filename : files_in_directory) {
        std::cout << filename << std::endl; // printed in alphabetical order
        fileList.push_back(filename);
    }

    if (files_in_directory.empty()) {
        std::cerr << "Name list empty" << std::endl;
        exit(-1);
    }

    //    std::cout << "Opening Dicom with " << files_in_directory.size() << " slices" << std::endl;

    const std::shared_ptr<WeightingFunction> wf(new Dicom(fileList));
//    const std::shared_ptr<WeightingFunction> wf(new Sphere(glm::vec3(100.0, 100.0, 100.0), 50.0));
//    const std::shared_ptr<WeightingFunction> wf(new Cube(glm::vec3(50.0, 50.0, 50.0), 100.0));
//    const std::shared_ptr<WeightingFunction> wf(new Cube(glm::vec3(10.0, 10.0, 10.0), 100.0));

    const std::shared_ptr<Sampler> sampler(new utk::SamplerDartThrowing);

//    const Grid grid(250, 100, 250, 1, wf, sampler, true);
//    const Grid grid(250, 80, 250, 1, wf, sampler, true);
    const Grid grid(10, 10, 10, 1, wf, sampler, true);
    BasicUS bu(grid);

    //    AcquisitionZone acq(glm::vec3(25, 50, 50), glm::angleAxis(0.f, glm::vec3(1, 0, 0)), 50, 1.1f, 60);
    //    AcquisitionZone acq(glm::vec3(0, 30, 125), glm::angleAxis(0.f, glm::vec3(0, 0, 1)), 256, 0.0, 256);

    //    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    //    stbi_write_jpg("test.jpg", 1024, 1024, 1, (void*)bu.getCorrespondingUS(acq, 1024, 1024).data(), 90);

    std::cout << "Ready to simulate" << std::endl;

    size_t iFrame = 0;
    while (true) {
        const auto& acq = inputPosStream.getAcquisition();
        outputPosStream << acq;

        float* translationData = (float*)acq.mData;
        float* quaternionData = (float*)&acq.mData[12];

        //        glm::vec3 pos(translationData[0], translationData[1], translationData[2]);
        const auto position = glm::make_vec3(translationData);
        const auto orientation = glm::make_quat(quaternionData);

        //    float* translation = glm::value_ptr(pos);
        //    float* orientation = glm::value_ptr(quat);
        //    memcpy(data, translation, 12);
        //    memcpy(&data[12], orientation, 16);

        const AcquisitionZone acqZone(position, orientation, 256, 256, 256);
//        AcquisitionZone acqZone(glm::vec3(0, 30, 125), glm::angleAxis(0.f, glm::vec3(0, 0, 1)), 256, 256, 256);
        const auto& image = bu.getCorrespondingUS(acqZone, height, width);
        assert(image.size() == width * height);
        const unsigned char* data = image.data();

        unsigned char transpose[256 * 256];
        for (int i = 0; i <256; ++i) {
            for (int j = 0; j <256; ++j) {
                transpose[i * 256 + j] = data[j * 256 + i];
            }
        }
        unsigned char verticalMirror[256 * 256];
        for (int i = 0; i <256; ++i) {
            for (int j = 0; j <256; ++j) {
                verticalMirror[i * 256 + j] = data[i * 256 + (256 - j - 1)];
            }
        }
//        outputScanStream << Stream::Acquisition { acq.mBackendTimestamp, acq.mBackendTimeOfArrival, verticalMirror, width * height };
        outputScanStream << Stream::Acquisition { acq.mBackendTimestamp, acq.mBackendTimeOfArrival, data, width * height };
//        outputScanStream << Stream::Acquisition { acq.mBackendTimestamp, acq.mBackendTimeOfArrival, transpose, width * height };

        //    stbi_write_jpg("test.jpg", 1024, 1024, 1, (void*)bu.getCorrespondingUS(acq, 1024, 1024).data(), 90);
        std::cout << "Computed frame " << iFrame << std::endl;
        ++iFrame;
    }

    return 0;
}
