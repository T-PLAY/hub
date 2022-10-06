
#include <cassert>
#include <string>

#include <DicomUtils/DicomUtils.hpp>
#include <IO/Stream.hpp>
#include <OutputSensor.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

int main( int argc, char* argv[] ) {
    unsigned int sliceWidth, sliceHeight, nSlices, bytePerVoxel;
    float pixelSpacingWidth, pixelSpacingHeight, sliceThickness;

    const auto filename = MRI_PATH "AXT2_ligaments_uterosacres/D0010525.dcm";
    //        const auto filename = MRI_PATH "Ax_T2_PROP_5MM/D0010275.dcm";
    //        const auto filename = MRI_PATH "Loc/D0010001.dcm"; // bad
    //        const auto filename = MRI_PATH "MPR_AX_T2/D0010551.dcm"; // bad
    //        const auto filename = MRI_PATH "MPR_Coro_T2/D0010710.dcm"; // bad
    //        const auto filename = MRI_PATH "MPR_Sag_T2/D0010652.dcm"; // bad
    //        const auto filename = MRI_PATH "PACS_AXIAL_ABDO/IM0001.dcm"; // CT
    //        const auto filename = MRI_PATH "PACS_AXIAL_AP/IM0001.dcm"; // CT
    //        const auto filename = MRI_PATH "PACS_AXIAL_TAP/IM0001.dcm"; // CT
    //        const auto filename = MRI_PATH "PACS_PARENCHYME/IM0001.dcm"; // CT
    //        const auto filename = MRI_PATH "Sag_CUBE_T2/D0010031.dcm";
    //        const auto filename = MRI_PATH "Scouts/IM0001.dcm";
    //        const auto filename = MRI_PATH "SerieSmartPrep/IM0001.dcm";
    //        const auto filename = MRI_PATH "WATER:AX_LAVA-Flex/D0010333.dcm";

    std::cout << "open '" << filename << " dicom file." << std::endl;

    std::string filenameStr = std::string( filename );
    std::string mriName     = filenameStr.substr( filenameStr.find( "MRI/" ) + 4 );
    mriName                 = mriName.substr( 0, mriName.find_last_of( '/' ) );
    std::cout << "mriName = '" << mriName << "'" << std::endl;

    auto* volumeData = DICOM::readDicomVolume( filename,
                                               &sliceWidth,
                                               &sliceHeight,
                                               &nSlices,
                                               &bytePerVoxel,
                                               &pixelSpacingWidth,
                                               &pixelSpacingHeight,
                                               &sliceThickness );

    assert( bytePerVoxel == 2 );

    glm::mat4 transform( 1.0 );
    const float scanWidth = sliceWidth * pixelSpacingWidth;
    const float scanDepth = sliceHeight * pixelSpacingHeight;
    transform = glm::scale( transform, glm::vec3( scanDepth / 2.0, 1.0, scanWidth / 2.0 ) );
    transform = glm::translate( transform, glm::vec3( 1.0, 100.0, 1.0 ) );
    //    transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(1.0, 0.0, 0.0));
    transform = glm::rotate( transform, glm::radians( 45.0f ), glm::vec3( 0.0, 1.0, 0.0 ) );
    //    transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));
    //    transform = glm::rotate(transform, glm::radians(90.0), glm::vec3(0.0, 1.0, 0.0));
    //        transform = glm::rotate(transform, glm::radians(90.0), glm::vec3(0.0, 1.0, 0.0));
    //    transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(0.0, 1.0, 0.0));
    const float* array = glm::value_ptr( transform );
    std::cout << "transform matrix : " << std::endl;
    for ( int i = 0; i < 4; ++i ) {
        for ( int j = 0; j < 4; ++j ) {
            std::cout << array[i * 4 + j] << " ";
        }
        std::cout << std::endl;
    }

    hub::SensorSpec::MetaData metaData;
    metaData["type"]      = "record";
    metaData["nAcqs"]     = nSlices;
    metaData["series"]    = mriName;
    metaData["parent"]    = "calibrator";
    metaData["transform"] = array;

    const int sliceSize = sliceWidth * sliceHeight * 2;

#ifdef USE_RGBA

    hub::SensorSpec sensorSpec( "mri",
                                { { { 1 }, hub::SensorSpec::Format::DOF6 },
                                  { { sliceWidth, sliceHeight }, hub::SensorSpec::Format::RGBA8 } },
                                std::move( metaData ) );
    const int volumeSize   = sliceSize * nSlices;
    const int textureSize  = sliceWidth * sliceHeight * 4;
    const auto& resolution = sensorSpec.m_resolutions.at( 1 );
    assert( hub::SensorSpec::computeAcquisitionSize( resolution ) == textureSize );
    unsigned char* texturesData = new unsigned char[textureSize * nSlices];
    for ( int i = 0; i < volumeSize / 2; ++i ) {
        //        auto * pixel = &volumeData[i * 2];
        //        unsigned char color = volumeData[2 * i + 1];
        //        unsigned char color = (volumeData[2 * i] + volumeData[2 * i + 1] * 256) / 256;
        unsigned char color = ( (uint16_t*)volumeData )[i] / 256;

        assert( 0 <= color && color <= 255 );

        texturesData[4 * i]     = color;
        texturesData[4 * i + 1] = color;
        texturesData[4 * i + 2] = color;
        texturesData[4 * i + 3] = color;
    }

#else
    unsigned char* texturesData = volumeData;
    const int textureSize       = sliceSize;
    hub::SensorSpec sensorSpec(
        "mri",
        { { { 1 }, hub::SensorSpec::Format::DOF6 },
          { { (int)sliceWidth, (int)sliceHeight }, hub::SensorSpec::Format::Y16 } },
        std::move( metaData ) );
#endif

    hub::OutputSensor outputSensor( sensorSpec, hub::io::OutputStream( "dicomStream" ) );
    for ( int iImage = 0; iImage < nSlices; ++iImage ) {
        hub::Dof6 dof6( 0.0, iImage * sliceThickness, 0.0 );
        //        glm::quat quat(1.0, 0.0, 0.0, 0.0);
        //        quat = glm::rotate(quat, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));
        hub::Measure image( &texturesData[textureSize * iImage], textureSize );
        outputSensor << ( hub::Acquisition { iImage, iImage } << std::move( dof6 )
                                                              << std::move( image ) );
    }

        hub::SensorSpec::MetaData metaData2;
        metaData2["parent"] = "calibrator";
        metaData2["transform"] = array;
        sensorSpec.m_metaData = metaData2;
        hub::OutputSensor outputSensor2( std::move( sensorSpec ),
                                        hub::io::OutputStream( "dicomStream2" ) );
        while ( true ) {
            for ( int iImage = 0; iImage < nSlices; ++iImage ) {
                hub::Dof6 dof6( 0.0, iImage * sliceThickness, 0.0 );
                hub::Measure image( &texturesData[textureSize * iImage], textureSize );
                outputSensor2 << ( hub::Acquisition { iImage, iImage } << std::move( dof6 )
                                          << std::move( image ) );
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            }
        }

//    while ( true ) {
//        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
//    }

    delete[] texturesData;

    return 0;
}
