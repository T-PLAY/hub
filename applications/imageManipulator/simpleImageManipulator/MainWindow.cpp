#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <filesystem>
#include <fstream>
#include <string>

#include <IO/File.hpp>
#include <InputSensor.hpp>

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

    {
//        std::string filepath = PROJECT_DIR "data/records/2022-05-27_10-16-00/webcam.txt";
        std::string filepath = PROJECT_DIR "data/records/2022-05-23_13-32-34 (carotide)/ULA-OP 256.txt";
        assert(std::filesystem::exists(filepath));
        std::fstream file(filepath, std::ios::binary | std::ios::in);

        hub::InputSensor inputStream(hub::io::File(std::move(file)));
        const auto & sensorSpec = inputStream.m_spec;
        const auto& acqs = inputStream.getAllAcquisitions();
        const auto& acq = acqs[4];
        assert(sensorSpec.m_resolutions.size() == 1);
        const auto& dims = sensorSpec.m_resolutions.at(0).first;
        const auto& format = sensorSpec.m_resolutions.at(0).second;

        //        ui->widgetStreamView->init(640, 480);
        m_streamView2D = new WidgetStreamView2D(this);
        //        m_streamView2D->setMinimumWidth(640);
        //        m_streamView2D->setMinimumHeight(480);
        m_streamView2D->init(dims.at(0), dims.at(1));
//        m_streamView2D->setData((unsigned char*)acq.m_data, sensorSpec.m_acquisitionSize, sensorSpec.m_dims, sensorSpec.m_format);
        m_streamView2D->setData((unsigned char*)acq.getMeasures().at(0).m_data, sensorSpec.m_acquisitionSize, dims, format);

        ui->mdiArea->addSubWindow(m_streamView2D);
    }

    {
        std::string filepath = PROJECT_DIR "data/records/2022-05-23_13-32-34 (carotide)/ULA-OP 256.txt";
        assert(std::filesystem::exists(filepath));
        std::fstream file(filepath, std::ios::binary | std::ios::in);

        hub::InputSensor inputStream(hub::io::File(std::move(file)));
        const auto & sensorSpec = inputStream.m_spec;
        const auto& acqs = inputStream.getAllAcquisitions();
        const auto& acq = acqs[163];
        assert(sensorSpec.m_resolutions.size() == 1);
        const auto& dims = sensorSpec.m_resolutions.at(0).first;
        const auto& format = sensorSpec.m_resolutions.at(0).second;

        //        ui->widgetStreamView->init(dims.at(0), dims.at(1));
        m_streamView2D_2 = new WidgetStreamView2D(this);
        m_streamView2D_2->init(512, 192, 35.0, 50.0);
//        m_streamView2D_2->init(512, 192);
//        m_streamView2D_2->setData((unsigned char*)acq.m_data, sensorSpec.m_acquisitionSize, sensorSpec.m_dims, sensorSpec.m_format);
        m_streamView2D->setData((unsigned char*)acq.getMeasures().at(0).m_data, sensorSpec.m_acquisitionSize, dims, format);
        ui->mdiArea->addSubWindow(m_streamView2D_2);

//    }


//    {
        m_imageManipulator = new FormImageManipulator(this);
        m_imageManipulator->setMinimumWidth(400);
        m_imageManipulator->setMinimumHeight(900);
        auto & widgetStreamView = m_imageManipulator->getWidgetStreamView();
        widgetStreamView.init(512, 192, 35.0, 50.0);
//        widgetStreamView.init(512, 192);
//        widgetStreamView.setData((unsigned char*)acq.m_data, sensorSpec.m_acquisitionSize, sensorSpec.m_dims, sensorSpec.m_format);
        widgetStreamView.setData((unsigned char*)acq.getMeasures().at(0).m_data, sensorSpec.m_acquisitionSize, dims, format);

    //    FormIm
        ui->mdiArea->addSubWindow(m_imageManipulator);

    }

}

MainWindow::~MainWindow()
{
    delete m_streamView2D;
    delete m_streamView2D_2;
    delete ui;
}
