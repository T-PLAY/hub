
//#include <iostream>
//#include <fstream>
#include <chrono>
//#include <glm/gtx/string_cast.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/quaternion.hpp>
//#include <glm/glm.hpp>

#include "Scus.h"

#define STB_IMAGE_IMPLEMENTATION
#include "utility/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "utility/stb_image_write.h"

int main( int argc, char* argv[] ) {
    //    std::string filename = MRI_PATH "AXT2_ligaments_uterosacres/D0010525.dcm";

    //    if (!std::filesystem::exists(filename)) {
    //        std::cout << "file '" << filename << "' doesn't exist" << std::endl;
    //        exit(1);
    //    }
    //    assert(std::filesystem::exists(filename));
    //    auto filepath = std::filesystem::path(filename).remove_filename();

    //    std::vector<std::filesystem::path> files_in_directory;
    //    for (const auto& entry : std::filesystem::directory_iterator(filepath.remove_filename()))
    //    {
    //        if (!std::filesystem::is_directory(entry)) {
    //            files_in_directory.push_back(entry);
    //        }
    //    }
    //    std::sort(files_in_directory.begin(), files_in_directory.end());

    //    std::vector<std::string> fileList;
    //    for (const auto& filename : files_in_directory) {
    //        std::cout << filename << std::endl; // printed in alphabetical order
    //        fileList.push_back(filename);
    //    }

    //    if (files_in_directory.empty()) {
    //        std::cerr << "Name list empty" << std::endl;
    //        exit(-1);
    //    }

    //    std::cout << "Opening Dicom with " << files_in_directory.size() << " slices" << std::endl;

    //    std::shared_ptr<WeightingFunction> wf(new Dicom(fileList));
    const std::shared_ptr<WeightingFunction> wf( new Cube( glm::vec3( 50.0, 50.0, 50.0 ), 100.0 ) );
    //    const std::shared_ptr<WeightingFunction> wf(new Sphere(glm::vec3(0.0, 0.0, 0.0), 50.0));

    std::shared_ptr<Sampler> sampler( new utk::SamplerDartThrowing );

    //    Grid grid(250, 80, 250, 1, wf, sampler, true);
    Grid grid( 10, 10, 10, 1, wf, sampler, true );
    BasicUS bu( grid );

    //        AcquisitionZone acq(glm::vec3(-20, 0, 0), glm::angleAxis(0.0f, glm::vec3(1, 0, 0)),
    //        200, 200, 200); AcquisitionZone acq(glm::vec3(-50, 0, 50), glm::quat(1.0, 0.0, 0.0,
    //        0.0), 200, 200, 200);

    glm::quat quat( 1.0, 0.0, 0.0, 0.0 );
    //    quat = glm::rotate(quat, glm::radians(25.0f), glm::vec3(1.0, 0.0, 0.0));
    //                    quat = glm::rotate(quat, glm::radians(moveSpeed), glm::vec3(0.0,
    //                    0.0, 1.0));
    //        AcquisitionZone acq(glm::vec3(-50, 1, 50), glm::angleAxis(glm::radians(20.0f),
    //        glm::vec3(0, 1, 0)), 200, 200, 200);
    AcquisitionZone acq( glm::vec3( 0, 1, 0 ), quat, 200, 200, 200 );
    //        AcquisitionZone acq(glm::vec3(80, 100, 0), glm::angleAxis(-90.f, glm::vec3(0, 0, 1)),
    //        200, 200, 200);
    //    AcquisitionZone acq(glm::vec3(100, 100, 50), glm::angleAxis(0.f, glm::vec3(1, 0, 0)), 256,
    //    256, 256);

    std::cout << "Grid done" << std::endl;
    //    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    const int width  = 512;
    const int height = 512;

    stbi_write_jpg( "test.jpg",
                    width,
                    height,
                    1,
                    (void*)bu.getCorrespondingUS( acq, width, height ).data(),
                    90 );
    std::cout << "Image done" << std::endl;

    return 0;
}
