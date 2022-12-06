#include "FormImageManipulator.h"
#include "ui_FormImageManipulator.h"

#include <filesystem>
#include <fstream>

#include <QPainter>
#include <QScrollAreaGrid.h>

FormImageManipulator::FormImageManipulator( QWidget* parent ) :
    QWidget( parent ), ui( new Ui::FormImageManipulator ) {
    ui->setupUi( this );

    ui->frame_top_2->setPixelPerUnit( &ui->widgetStreamView_2->getCanvasPixelPerUnit() );
    ui->frame_top_2->setType( FrameRuler::Type::HORIZONTAL );
    ui->frame_top_2->setScrollBar( ui->scrollArea->horizontalScrollBar() );
    ui->frame_top_2->setCanvasPixelSize( &ui->widgetStreamView_2->getCanvasPixelWidth() );

    ui->frame_left_2->setPixelPerUnit( &ui->widgetStreamView_2->getCanvasPixelPerUnit() );
    ui->frame_left_2->setType( FrameRuler::Type::VERTICAL );
    ui->frame_left_2->setScrollBar( ui->scrollArea->verticalScrollBar() );
    ui->frame_left_2->setCanvasPixelSize( &ui->widgetStreamView_2->getCanvasPixelHeight() );

    ui->scrollArea->setCanvasPixelPerUnit( &ui->widgetStreamView_2->getCanvasPixelPerUnit() );

    ui->scrollArea->setScrollAreaLeft( ui->scrollArea_left );
    ui->scrollArea->setScrollAreaTop( ui->scrollArea_top );

    ui->widgetStreamView_2->setShowGrid( true );

    ui->scrollAreaWidgetContents_grid->setCanvasPixelPerUnit(
        &ui->widgetStreamView_2->getCanvasPixelPerUnit() );
    ui->scrollAreaWidgetContents_grid->setImageUnitWidth(
        &ui->widgetStreamView_2->getImageUnitWidth() );
    ui->scrollAreaWidgetContents_grid->setImageUnitHeight(
        &ui->widgetStreamView_2->getImageUnitHeight() );
    ui->scrollAreaWidgetContents_grid->setCanvasPixelWidth(
        &ui->widgetStreamView_2->getCanvasPixelWidth() );
    ui->scrollAreaWidgetContents_grid->setCanvasPixelHeight(
        &ui->widgetStreamView_2->getCanvasPixelHeight() );

    QObject::connect( ui->scrollArea,
                      &QScrollAreaGrid::pixelPerUnitChanged,
                      ui->widgetStreamView_2,
                      &WidgetStreamView2D::onPixelPerUnitChanged );
    QObject::connect( ui->scrollArea,
                      &QScrollAreaGrid::pixelPerUnitChanged,
                      ui->scrollAreaWidgetContents_grid,
                      &WidgetGrid::onPixelPerUnitChanged );
    QObject::connect( ui->scrollArea,
                      &QScrollAreaGrid::pixelPerUnitChanged,
                      ui->frame_top_2,
                      &FrameRuler::onPixelPerUnitChanged );
    QObject::connect( ui->scrollArea,
                      &QScrollAreaGrid::pixelPerUnitChanged,
                      ui->frame_left_2,
                      &FrameRuler::onPixelPerUnitChanged );

    QObject::connect( ui->widgetStreamView_2,
                      &WidgetStreamView2D::inited,
                      this,
                      &FormImageManipulator::onStreamViewInited );


}

FormImageManipulator::~FormImageManipulator() {
    delete ui;
}


void FormImageManipulator::init() {
    ui->widgetStreamView_2->setData(
        nullptr, 256 * 256, { 256, 256 }, hub::Format::Y8 );
}

void FormImageManipulator::onStreamViewInited() {
    emit ui->scrollArea->pixelPerUnitChanged();
}


WidgetStreamView2D& FormImageManipulator::getWidgetStreamView() {
    return *ui->widgetStreamView_2;
}

