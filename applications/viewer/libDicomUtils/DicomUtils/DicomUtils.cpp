#include <DicomUtils/DicomUtils.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmimgle/dcmimage.h>

#include <cassert>

#include <filesystem>

    namespace DICOM {

//        void getDicomPixels(const std::string& filename, std::vector<unsigned char>& res)
//        {
//            DicomImage image(filename.c_str());

//            if (image.getStatus() == EIS_Normal) {
//                if (image.isMonochrome()) {

//                    image.setMinMaxWindow();

//                    auto* pixelData = (Uint8*)(image.getOutputData(8 /* bits */));

//                    res.clear();
//                    res.insert(res.begin(), pixelData, pixelData + image.getHeight() * image.getWidth());
//                }
//            } else {
//                std::cerr << "Error: cannot load DICOM image (" << DicomImage::getString(image.getStatus()) << ")" << std::endl;
//                exit(-1);
//            }
//        }

        // read a DICOM volume
        unsigned char* readDicomVolume(const char* filename,
            unsigned int* imageWidth,
            unsigned int* imageHeight,
            unsigned int* nImages,
            unsigned int* nBytesPerVoxel,
            float* pixelSpacingWidth,
            float* pixelSpacingHeight,
            float* sliceThickness)
        {
            if (! std::filesystem::exists(filename)) {
                std::cout << "file '" << filename << "' doesn't exist" << std::endl;
                exit(1);
            }
            assert(std::filesystem::exists(filename));
            auto filepath = std::filesystem::path(filename).remove_filename();

            std::vector<std::filesystem::path> files_in_directory;
//            int i = 0;
            for (const auto & entry : std::filesystem::directory_iterator(filepath.remove_filename())) {
//                if (i >= 1)
//                    break;
                if (! std::filesystem::is_directory(entry)) {
                    files_in_directory.push_back(entry);
                }
//                ++i;
            }
//            std::copy(std::filesystem::directory_iterator(filepath), std::filesystem::directory_iterator(), std::back_inserter(files_in_directory));
            std::sort(files_in_directory.begin(), files_in_directory.end());

//            for (const auto & filename : files_in_directory) {
//                std::cout << filename << std::endl; // printed in alphabetical order
//            }

//            std::vector<std::string> names;
//            names.push_back(filename);

            if (files_in_directory.empty()) {
                std::cerr << "Name list empty" << std::endl;
                exit(-1);
            }

            std::cout << "Opening Dicom with " << files_in_directory.size() << " slices" << std::endl;

            DcmFileFormat file_format;
            OFCondition status = file_format.loadFile(files_in_directory.front().string().c_str());

            if (status.bad()) {
                std::cerr << "Problem openning file:" << files_in_directory.front().string().c_str() << std::endl;
                exit(-1);
            }
            DcmDataset* dataset = file_format.getDataset();

            OFString property;
            OFCondition condition;

            condition = dataset->findAndGetOFStringArray(DCM_PixelSpacing, property);
            if (condition.good()) {
                std::cout << "Pixel spacing: " << property << std::endl;
            } else {
                std::cerr << "Could not get Pixel spacing" << std::endl;
                exit(-1);
            }
            *pixelSpacingWidth = std::stof(property.substr(0, property.find("\\")).c_str());
            *pixelSpacingHeight = std::stof(property.substr(property.find("\\") + 1, property.size()).c_str());

            condition = dataset->findAndGetOFStringArray(DCM_SliceThickness, property);
            if (condition.good()) {
                std::cout << "Slice Thickness: " << property << std::endl;
            } else {
                std::cerr << "Could not get Slice Thickness" << std::endl;
                exit(-1);
            }
            *sliceThickness = std::stof(property.c_str());

            condition = dataset->findAndGetOFStringArray(DCM_Columns, property);
            if (condition.good()) {
                std::cout << "Columns: " << property << std::endl;
            } else {
                std::cerr << "Could not get Columns" << std::endl;
                exit(-1);
            }
            *imageWidth = std::stoi(property.c_str());

            condition = dataset->findAndGetOFStringArray(DCM_Rows, property);
            if (condition.good()) {
                std::cout << "Rows : " << property << std::endl;
            } else {
                std::cerr << "Could not get Rows" << std::endl;
                exit(-1);
            }
            *imageHeight = std::stoi(property.c_str());

            *nImages = files_in_directory.size();

            const auto realWidth = float(*imageWidth) * *pixelSpacingWidth;
            const auto realHeight = float(*imageHeight) * *pixelSpacingHeight;
            const auto realThickness = *sliceThickness * *nImages;
            std::cout << "Covered dimensions are: {x: " << realHeight << ", y: " << realThickness << ", z: " << realWidth << "}" << std::endl;

//            numc = 4;
            *nBytesPerVoxel = 2;
            const auto imageSize = *imageWidth * *imageHeight * *nBytesPerVoxel;
            auto * volume = new unsigned char[imageSize * *nImages];

            for (int i = 0; i < *nImages; ++i) {
                const auto filename = files_in_directory.at(i);

//                DicomImage image(filename.c_str());
                DicomImage image(filename.string().c_str());

                auto dbg = image.getDepth();
//                assert(image.getDepth() == *nBytesPerVoxel * 8);
                dbg = image.getFrameCount();
                assert(image.getFrameCount() == 1);
                dbg = image.getNumberOfFrames();
                assert(image.getNumberOfFrames() == 1);
                dbg = image.getWidth();
                assert(image.getWidth() == *imageWidth);
                dbg = image.getHeight();
                assert(image.getHeight() == *imageHeight);

                if (image.getStatus() == EIS_Normal) {
                    assert(image.isMonochrome());

                    image.setMinMaxWindow();

//                    Uint8* pixelData = (Uint8*)(image.getOutputData(8 /* bits */));
//                    assert(pixelData != NULL);

                    image.getOutputData(&volume[i * imageSize], imageSize, *nBytesPerVoxel * 8);

                    //                    res.clear();
                    //                    res.insert(res.begin(), pixelData, pixelData + image.getHeight() * image.getWidth());
                } else {
                    std::cerr << "Error: cannot load DICOM image (" << DicomImage::getString(image.getStatus()) << ")" << std::endl;
                    exit(-1);
                }

//                std::cout << "[DicomUtils] loaded image " << i << " '" << filename << std::endl;
            }


            return volume;
        }

    } // namespace DICOM

