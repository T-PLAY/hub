#include "FormImageManipulator.h"
#include "ui_FormImageManipulator.h"

#include <FileIO.h>
#include <filesystem>
#include <fstream>
#include <stream.h>

#include <QPainter>
#include <QScrollAreaGrid.h>

FormImageManipulator::FormImageManipulator(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::FormImageManipulator)
{
    ui->setupUi(this);

    ui->frame_top_2->setPixelPerUnit(&ui->widgetStreamView_2->getCanvasPixelPerUnit());
    ui->frame_top_2->setType(FrameRuler::Type::HORIZONTAL);
    ui->frame_top_2->setScrollBar(ui->scrollArea->horizontalScrollBar());
    ui->frame_top_2->setCanvasPixelSize(&ui->widgetStreamView_2->getCanvasPixelWidth());

    ui->frame_left_2->setPixelPerUnit(&ui->widgetStreamView_2->getCanvasPixelPerUnit());
    ui->frame_left_2->setType(FrameRuler::Type::VERTICAL);
    ui->frame_left_2->setScrollBar(ui->scrollArea->verticalScrollBar());
    ui->frame_left_2->setCanvasPixelSize(&ui->widgetStreamView_2->getCanvasPixelHeight());

//    ui->widgetStreamView_2->setCanvasPixelPerUnit(ui->scrollArea->getCanvasPixelPerUnit());
    ui->scrollArea->setCanvasPixelPerUnit(&ui->widgetStreamView_2->getCanvasPixelPerUnit());

    ui->scrollArea->setScrollAreaLeft(ui->scrollArea_left);
    ui->scrollArea->setScrollAreaTop(ui->scrollArea_top);
    QObject::connect(ui->scrollArea, &QScrollAreaGrid::pixelPerUnitChanged, ui->widgetStreamView_2, &WidgetStreamView2D::onPixelPerUnitChanged);
    QObject::connect(ui->scrollArea, &QScrollAreaGrid::pixelPerUnitChanged, ui->scrollAreaWidgetContents_grid, &WidgetGrid::onPixelPerUnitChanged);
    QObject::connect(ui->scrollArea, &QScrollAreaGrid::pixelPerUnitChanged, ui->frame_top_2, &FrameRuler::onPixelPerUnitChanged);
    QObject::connect(ui->scrollArea, &QScrollAreaGrid::pixelPerUnitChanged, ui->frame_left_2, &FrameRuler::onPixelPerUnitChanged);


    ui->widgetStreamView_2->setShowGrid(true);

    ui->scrollAreaWidgetContents_grid->setCanvasPixelPerUnit(&ui->widgetStreamView_2->getCanvasPixelPerUnit());
    ui->scrollAreaWidgetContents_grid->setImageUnitWidth(&ui->widgetStreamView_2->getImageUnitWidth());
    ui->scrollAreaWidgetContents_grid->setImageUnitHeight(&ui->widgetStreamView_2->getImageUnitHeight());
    ui->scrollAreaWidgetContents_grid->setCanvasPixelWidth(&ui->widgetStreamView_2->getCanvasPixelWidth());
    ui->scrollAreaWidgetContents_grid->setCanvasPixelHeight(&ui->widgetStreamView_2->getCanvasPixelHeight());


    ui->scrollArea->update();
//    if (true)
//    if (false)
//    {

//        std::string filepath = PROJECT_DIR "data/records/2022-05-27_10-16-00/webcam.txt";
//        assert(std::filesystem::exists(filepath));
//        std::fstream file(filepath, std::ios::binary | std::ios::in);

//        InputStream inputStream(FileIO(std::move(file)));
//        const auto& acqs = inputStream.getAllAcquisition();
//        const auto& acq = acqs[4];
//        const auto& dims = inputStream.getDims();

////        ui->widgetStreamView->init(640, 480);
//        ui->widgetStreamView_2->init(dims.at(0), dims.at(1));
//        ui->widgetStreamView_2->setData((unsigned char*)acq.mData, inputStream.getAcquisitionSize(), inputStream.getDims(), inputStream.getFormat());

//    } else {

//        std::string filepath = PROJECT_DIR "data/records/2022-05-23_13-32-34 (carotide)/ULA-OP 256.txt";
//        assert(std::filesystem::exists(filepath));
//        std::fstream file(filepath, std::ios::binary | std::ios::in);

//        InputStream inputStream(FileIO(std::move(file)));
//        const auto& acqs = inputStream.getAllAcquisition();
//        const auto& acq = acqs[163];
//        const auto& dims = inputStream.getDims();

////        ui->widgetStreamView->init(dims.at(0), dims.at(1));
//                ui->widgetStreamView_2->init(512, 192, 35.0, 50.0);
//        ui->widgetStreamView_2->setData((unsigned char*)acq.mData, inputStream.getAcquisitionSize(), inputStream.getDims(), inputStream.getFormat());
//    }

    //    constexpr int imgWidth = 640;
    //    constexpr int imgHeight = 480;
    //    constexpr int imgSize = imgWidth * imgHeight;

    //    {
    //        unsigned char data[imgSize];
    ////        for (int i = 0; i < imgWidth; ++i) {
    ////            for (int j = 0; j < imgHeight; ++j) {
    ////                data[i * imgHeight + j] = int(255.0 * double(i + j) / (imgWidth + imgHeight)) % 255;
    ////            }
    ////        }
    //                for (int i = 0; i < imgSize; ++i) {
    //                    data[i] = ((double)i / imgSize) * 255.0;
    //                }

    ////        ui->widgetStreamView->init(imgWidth, imgHeight, 50.0, 35.0);
    //        ui->widgetStreamView->init(imgWidth, imgHeight);
    //        ui->widgetStreamView->setData((unsigned char*)data, imgSize, { imgWidth, imgHeight }, Stream::Format::Y8);
    ////        ui->widgetStreamView->update();
    //    }
}

FormImageManipulator::~FormImageManipulator()
{
    delete ui;
}

void FormImageManipulator::update(const Stream::Acquisition &acquisition)
{
    ui->widgetStreamView_2->setData((unsigned char*)acquisition.mData, {512, 192}, Stream::Format::Y8);

}

void FormImageManipulator::init()
{
    ui->widgetStreamView_2->setData(nullptr, {512, 192}, Stream::Format::Y8);

}

WidgetStreamView2D &FormImageManipulator::getWidgetStreamView()
{
    return *ui->widgetStreamView_2;
}

// void FormImageManipulator::paintEvent(QPaintEvent* event)
//{
//     std::cout << "[FormImageManipulator] paintEvent" << std::endl;

//    QPainter painter(this);
//    //    painter.setRenderHint(QPainter::Antialiasing);

//    //    const int widthPixel = ui->widgetStreamView->width();
//    //    const int widthPixel = ui->frame_top->width();
//    //    for (int i = 0; i < 10; ++i) {
//    //        painter.drawText(i * 10, 10, QString::number(i));
//    //    }
//    //    const int widthMillimeters =

//    //        double pixelHeightPerMillimeters = height / m_realWidth;
//}
