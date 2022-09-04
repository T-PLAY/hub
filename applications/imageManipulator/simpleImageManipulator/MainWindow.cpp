#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <filesystem>
#include <fstream>
#include <string>

#include <IO/File.hpp>
#include <InputSensor.hpp>
#include <DicomLoader/DicomUtils.hpp>
#include <IO/Stream.hpp>
#include <OutputSensor.hpp>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //    m_imageManipulator = new FormImageManipulator(this);
    //    m_imageManipulator->setMinimumWidth(640);
    //    m_imageManipulator->setMinimumHeight(1000);

    //    FormIm
    //    ui->mdiArea->addSubWindow(m_imageManipulator);

//    {
    const auto filename = MRI_PATH "AXT2_ligaments_uterosacres/D0010525.dcm";
//            const auto filename = MRI_PATH "Ax_T2_PROP_5MM/D0010275.dcm";
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
    std::string filenameStr = std::string( filename );
    std::string mriName     = filenameStr.substr( filenameStr.find( "MRI/" ) + 4 );
    mriName                 = mriName.substr( 0, mriName.find_last_of( '/' ) );
    std::cout << "mriName = '" << mriName << "'" << std::endl;

    //    if (false)
//    {
        std::cout << "open '" << filename << " dicom file." << std::endl;

        unsigned int sliceWidth, sliceHeight, nSlices, bytePerVoxel;
        float pixelSpacingWidth, pixelSpacingHeight, sliceThickness;
        auto* volumeData = Ra::IO::DICOM::readDicomVolume( filename,
                                                           &sliceWidth,
                                                           &sliceHeight,
                                                           &nSlices,
                                                           &bytePerVoxel,
                                                           &pixelSpacingWidth,
                                                           &pixelSpacingHeight,
                                                           &sliceThickness );

        assert( bytePerVoxel == 2 );


        const int sliceSize = sliceWidth * sliceHeight * 2;
        unsigned char* texturesData = volumeData;

////        std::string filepath = PROJECT_DIR "data/records/2022-05-27_10-16-00/webcam.txt";
//        std::string filepath = PROJECT_DIR "data/records/2022-05-23_13-32-34 (carotide)/ULA-OP 256.txt";
//        assert(std::filesystem::exists(filepath));
//        std::fstream file(filepath, std::ios::binary | std::ios::in);

//        hub::InputSensor inputStream(hub::io::File(std::move(file)));
//        const auto & sensorSpec = inputStream.m_spec;
//        const auto& acqs = inputStream.getAllAcquisitions();
//        const auto& acq = acqs[4];
//        assert(sensorSpec.m_resolutions.size() == 1);
//        const auto& dims = sensorSpec.m_resolutions.at(0).first;
//        const auto& format = sensorSpec.m_resolutions.at(0).second;

        //        ui->widgetStreamView->init(640, 480);
        m_streamView2D = new WidgetStreamView2D(this);
        //        m_streamView2D->setMinimumWidth(640);
        //        m_streamView2D->setMinimumHeight(480);
        m_streamView2D->init(sliceWidth, sliceHeight);
//        m_streamView2D->setData((unsigned char*)acq.m_data, sensorSpec.m_acquisitionSize, sensorSpec.m_dims, sensorSpec.m_format);
//        m_streamView2D->setData((unsigned char*)acq.getMeasures().at(0).m_data, sensorSpec.m_acquisitionSize, dims, format);
//        m_streamView2D->setData((unsigned char*)acq.getMeasures().at(0).m_data, sensorSpec.m_acquisitionSize, dims, format);
        m_streamView2D->setData(texturesData, sliceSize, {(int)sliceWidth, (int)sliceHeight}, hub::SensorSpec::Format::Y16);

        ui->mdiArea->addSubWindow(m_streamView2D);
//    }

//    {
//        std::string filepath = PROJECT_DIR "data/records/2022-05-23_13-32-34 (carotide)/ULA-OP 256.txt";
//        assert(std::filesystem::exists(filepath));
//        std::fstream file(filepath, std::ios::binary | std::ios::in);

//        hub::InputSensor inputStream(hub::io::File(std::move(file)));
//        const auto & sensorSpec = inputStream.m_spec;
//        const auto& acqs = inputStream.getAllAcquisitions();
//        const auto& acq = acqs[163];
//        assert(sensorSpec.m_resolutions.size() == 1);
//        const auto& dims = sensorSpec.m_resolutions.at(0).first;
//        const auto& format = sensorSpec.m_resolutions.at(0).second;

//        //        ui->widgetStreamView->init(dims.at(0), dims.at(1));
//        m_streamView2D_2 = new WidgetStreamView2D(this);
//        m_streamView2D_2->init(512, 192, 35.0, 50.0);
////        m_streamView2D_2->init(512, 192);
////        m_streamView2D_2->setData((unsigned char*)acq.m_data, sensorSpec.m_acquisitionSize, sensorSpec.m_dims, sensorSpec.m_format);
//        m_streamView2D->setData((unsigned char*)acq.getMeasures().at(0).m_data, sensorSpec.m_acquisitionSize, dims, format);
//        ui->mdiArea->addSubWindow(m_streamView2D_2);

////    }


//    {
        m_imageManipulator = new FormImageManipulator(this);
        m_imageManipulator->setMinimumWidth(400);
        m_imageManipulator->setMinimumHeight(900);
        auto & widgetStreamView = m_imageManipulator->getWidgetStreamView();
        widgetStreamView.init(sliceWidth, sliceHeight, 35.0, 50.0);
//        widgetStreamView.init(512, 192);
//        widgetStreamView.setData((unsigned char*)acq.m_data, sensorSpec.m_acquisitionSize, sensorSpec.m_dims, sensorSpec.m_format);
        widgetStreamView.setData(texturesData, sliceSize, {(int)sliceWidth, (int)sliceHeight}, hub::SensorSpec::Format::Y16);

    //    FormIm
        ui->mdiArea->addSubWindow(m_imageManipulator);

//    }

}

MainWindow::~MainWindow()
{
    delete m_streamView2D;
    delete m_streamView2D_2;
    delete ui;
}
