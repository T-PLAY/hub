
#include <iostream>
#include <fstream>
#include <chrono>
#include <glm/gtx/string_cast.hpp>

#include "Scus.h"

#define STB_IMAGE_IMPLEMENTATION
#include "utility/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "utility/stb_image_write.h"

int main(int argc, char *argv[]) {

    if(argc < 2) {
        std::cerr << "Settings file missing" << std::endl;
        return -1;
    }

    std::string jsonName(argv[1]);
    std::ifstream in(jsonName);
    if(!in.is_open() || in.fail()) {
        std::cerr << "Could not open settings file : " << jsonName << std::endl;
        return -1;
    }

    nlohmann::json jf = nlohmann::json::parse(in);

    Grid grid = Grid::fromJson(jf.at("Grid"));
    BasicUS bu(grid);
    ScatMap sm(grid);
    RealScatMap rsm;

    if(jf.at("AcquisitionZone").contains("Type") && jf.at("AcquisitionZone").at("Type") == "Single") {
        //Single acqzone
        AcquisitionZone aq = AcquisitionZone::fromJson(jf.at("AcquisitionZone"));
        PointWriter::WritePointsRaw(grid,aq,"raw_res.dat");
        PointWriter::WritePoints(grid,aq, VisualisationMode::SPACE, "res.dat");

//        std::string realname("rus2.jpg");
//        stbi_write_jpg(realname.c_str(), 500, 600, 1, (void *) rsm.getCorrespondingAbsScats(aq, grid, 500, 600).data(),
//                       90);
//        std::string name("us3.jpg");
//        stbi_write_jpg(name.c_str(), 500, 600, 1, (void *) bu.getCorrespondingUS(aq, 500, 600).data(),
//                       90);
////
//        std::string name2("scatmap2.jpg");
//        stbi_write_jpg(name2.c_str(), 500, 600, 1, (void *) sm.getCorrespondingScatMap(aq, 500, 600).data(), 90);


    } else if(jf.at("AcquisitionZone").contains("Type") && jf.at("AcquisitionZone").at("Type") == "Extern"){
        //Acqzone positions stored in extern file
        float width = jf.at("AcquisitionZone").at("Dimension").at("Width");
        float height = jf.at("AcquisitionZone").at("Dimension").at("Height");
        float depth = jf.at("AcquisitionZone").at("Dimension").at("Depth");

        std::ifstream acqfile(std::string(jf.at("AcquisitionZone").at("File")));
        if(!acqfile.is_open() || acqfile.fail()) {
            std::cerr << "Could not open acq file : " << std::string(jf.at("AcquisitionZone").at("File")) << std::endl;
            return -1;
        }

//        Grid grid = Grid::fromJson(jf.at("Grid"));
//        BasicUS bu(grid);
//        ScatMap sm(grid);

        int i = 0;
        std::vector<AcquisitionZone> acqs;
        std::string line;
        for(; std::getline(acqfile, line);) {
            if(!line.empty() && line[0] != '%' && line[0] != '\n') {
//                Grid grid = Grid::fromJson(jf.at("Grid"));
                acqs.emplace_back(AcquisitionZone::fromText(line.substr(0, line.size() - 1), width, height, depth));
//                std::string name("us");
//                name += std::to_string(i) + ".jpg";
//                stbi_write_jpg(name.c_str(), 500, 500, 1, (void *) bu.getCorrespondingUS(acqs.back(), 500, 500).data(),
//                             90);
//
//                std::string name2("scatmap");
//                name2 += std::to_string(i) + ".jpg";
//                stbi_write_jpg(name2.c_str(), 500, 500, 1, (void *) sm.getCorrespondingScatMap(acqs.back(), 500, 500).data(), 90);

                std::string name3("corr5/res");
                name3 += std::to_string(i) + ".dat";

                PointWriter::WritePointsRaw(grid,acqs.back(),name3);
                //PointWriter::WritePoints(grid,acqs.back(), VisualisationMode::SPACE, name3);

                ++i;
            }
        }

    } else {
        std::cerr << "Acquisition Type missing" << std::endl;
        return -1;
    }


    return 0;
}

//-0.707107 0 0 0.707107


//std::string jsonName("../src/settings3.json");
//std::ifstream in(jsonName);
//if(!in.is_open() || in.fail()) {
//std::cerr << "Could not open settings file : " << jsonName << std::endl;
//exit(-1);
//}
//
//nlohmann::json jf = nlohmann::json::parse(in);
//
//std::cout << "Json read" << std::endl;
//
//Grid grid = Grid::fromJson(jf.at("Grid"));
//std::cout << "Grid created" << std::endl;
//AcquisitionZone acquisitionZone = AcquisitionZone::fromJson(jf.at("AcquisitionZone"));
//std::cout << "Acquisition Zone created" << std::endl;
//
//ScatMap sm(grid.weightingFunction());
//
//std::cout << "ScatMap object created" << std::endl;
//
//auto start = std::chrono::high_resolution_clock::now();
//std::vector<unsigned char> res = sm.getCorrespondingScatMap(acquisitionZone,500,600);
//auto stop = std::chrono::high_resolution_clock::now();
//auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
//
//res.shrink_to_fit();
//std::cout << "Scat Map generated in " << duration.count() << " microseconds" << std::endl;
//jo_write_jpg("scatmap.jpg",(void *)res.data(),500,600,1,90);
//std::cout << "Scat Map saved" << std::endl;
//
//start = std::chrono::high_resolution_clock::now();
//std::vector<double> restemp = sm.getCorrespondingScats(acquisitionZone,500,600);
//stop = std::chrono::high_resolution_clock::now();
//duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
//
//res.clear();
//res.reserve(restemp.size());
//for(auto& e : restemp) {
//res.emplace_back(round(255*std::abs(e)));
//}
//
//res.shrink_to_fit();
//std::cout << "Scats generated in " << duration.count() << " microseconds" << std::endl;
//jo_write_jpg("base2.jpg",(void *)res.data(),500,600,1,90);
//std::cout << "Scats Saved" << std::endl;
//res.clear();
//
//BasicUS bu(grid.weightingFunction());
//std::cout << "BasicUS created" << std::endl;
//
//start = std::chrono::high_resolution_clock::now();
//res = bu.getCorrespondingUS(acquisitionZone,500,600);
//stop = std::chrono::high_resolution_clock::now();
//duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
//
//std::cout << "BasicUS generated in " << duration.count() << " microseconds" << std::endl;
//
//jo_write_jpg("us.jpg",(void *)res.data(),50,60,1,90);
//std::cout << "US  saved : " << res.size() << std::endl;

//    PointWriter::WritePointsFromJson(jsonName);
