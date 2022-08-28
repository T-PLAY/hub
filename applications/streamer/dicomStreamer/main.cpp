
#include <cassert>

#include <DicomLoader/DicomUtils.hpp>
#include <IO/Stream.hpp>
#include <OutputSensor.hpp>

int main( int argc, char* argv[] ) {
    unsigned int imageWidth, imageHeight, nImages, bytePerVoxel;
    float pixelSpacingWidth, pixelSpacingHeight, sliceThickness;

    const auto filename = MRI_PATH "AXT2_ligaments_uterosacres/D0010525.dcm";
    //        app.loadFile(QString(MRI_PATH "Ax_T2_PROP_5MM/D0010275.dcm"));
    //    app.loadFile(QString(MRI_PATH "Loc/D0010001.dcm"));
    //    app.loadFile(QString(MRI_PATH "MPR_AX_T2/D0010551.dcm"));
    //    app.loadFile(QString(MRI_PATH "MPR_Coro_T2/D0010710.dcm"));
    //    app.loadFile(QString(MRI_PATH "MPR_Sag_T2/D0010652.dcm"));
    //    app.loadFile(QString(MRI_PATH "PACS_AXIAL_ABDO/IM0001.dcm"));
    //    app.loadFile(QString(MRI_PATH "PACS_AXIAL_AP/IM0001.dcm"));
    //    app.loadFile(QString(MRI_PATH "PACS_AXIAL_TAP/IM0001.dcm"));
    //    app.loadFile(QString(MRI_PATH "PACS_PARENCHYME/IM0001.dcm"));
    //    app.loadFile(QString(MRI_PATH "RapportDose/IM0001.dcm"));
    //    app.loadFile(QString(MRI_PATH "Sag_CUBE_T2/D0010031.dcm"));
    //    app.loadFile(QString(MRI_PATH "Scouts/IM0001.dcm"));
    //    app.loadFile(QString(MRI_PATH "ScreenSave/IM0001.dcm"));
    //    app.loadFile(QString(MRI_PATH "SerieSmartPrep/IM0001.dcm"));
    //    app.loadFile(QString(MRI_PATH "WATER:AX_LAVA-Flex/D0010333.dcm"));

    auto* volumeData = Ra::IO::DICOM::readDicomVolume( filename,
                                                       &imageWidth,
                                                       &imageHeight,
                                                       &nImages,
                                                       &bytePerVoxel,
                                                       &pixelSpacingWidth,
                                                       &pixelSpacingHeight,
                                                       &sliceThickness );

    assert( imageWidth == 512 );
    assert( imageHeight == 512 );
    assert( bytePerVoxel == 2 );

    //    auto& myTexture = app.m_engine->getTextureManager()->addTexture("myTexture", 512, 512,
    //    volumeData);

    hub::SensorSpec::MetaData metaData;
    metaData["type"]        = "record";
    constexpr int imageSize = 512 * 512 * 2;
    hub::SensorSpec sensorSpec( "mri",
                                { { { 1 }, hub::SensorSpec::Format::DOF6 },
                                  { { 512, 512 }, hub::SensorSpec::Format::Y16 } },
                                std::move( metaData ) );
    hub::OutputSensor outputSensor( std::move( sensorSpec ),
                                    hub::io::OutputStream( "dicomStream" ) );

//    nImages = 1;
    for ( int iImage = 0; iImage < nImages; ++iImage ) {
        hub::Dof6 dof6( 0.0, iImage * sliceThickness, 0.0 );
        //        dof6.m_z = iImage * sliceThickness;
        hub::Measure image( &volumeData[imageSize * iImage], imageSize );
        //        outputSensor << hub::Acquisition { iImage, iImage, &volumeData[imageSize *
        //        iImage], imageSize }; hub::Acquisition acq(iImage, iImage); hub::Acquisition {0,
        //        0} << std::move(dof6); outputSensor << (hub::Acquisition { iImage, iImage } <<
        //        std::move(dof6) << std::move(image)); outputSensor << hub::Acquisition { iImage,
        //        iImage, std::move(dof6) };
        outputSensor << ( hub::Acquisition { iImage, iImage } << std::move( dof6 )
                                                              << std::move( image ) );
    }

    while ( true ) {
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }

    return 0;
}
