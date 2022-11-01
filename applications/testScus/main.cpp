
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






    const std::shared_ptr<WeightingFunction> wf( new Cube( glm::vec3( 50.0, 50.0, 50.0 ), 100.0 ) );

    std::shared_ptr<Sampler> sampler( new utk::SamplerDartThrowing );

    Grid grid( 10, 10, 10, 1, wf, sampler, true );
    BasicUS bu( grid );


    glm::quat quat( 1.0, 0.0, 0.0, 0.0 );
    AcquisitionZone acq( glm::vec3( 0, 1, 0 ), quat, 200, 200, 200 );

    std::cout << "Grid done" << std::endl;
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
