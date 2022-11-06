#include "FormStreamView.h"
#include "ui_FormStreamView.h"

#include <cmath>
#include <iostream>

#include <QMdiSubWindow>
#include <QLineEdit>

#include <WidgetStreamView.h>

FormStreamView::FormStreamView(std::string streamName,
                                const hub::SensorSpec& sensorSpec, const std::string &syncStreamName,
                                QStringListModel& sensorModel,
                                QWidget* parent )

    :
    QWidget( parent ),
    ui( new Ui::FormStreamView )
#ifndef USE_COMPLETE_VIEWER
    ,
    m_streamName( streamName ),
    m_sensorSpec( sensorSpec ),
    mSensorModel( sensorModel )
#endif
{
    ui->setupUi( this );

#ifdef USE_COMPLETE_VIEWER
    ui->frameButtonOnOff->setVisible(false);
//    ui->comboBox_syncSensor->setCurrentText("none");
//    QLineEdit * lineEdit = new QLineEdit("none");
//    lineEdit->setReadOnly(true);
//    ui->comboBox_syncSensor->setLineEdit(lineEdit);
//    ui->comboBox_syncSensor->setEnabled(false);
//    ui->comboBox_syncSensor->setVisible(false);
    QLabel * label = new QLabel((syncStreamName == "") ?("none") :(syncStreamName.c_str()));
    label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
//    label->setAlignment(Qt::AlignLeft);
    ui->horizontalLayout_syncStream->insertWidget(1, label);
    ui->horizontalLayout_syncStream->removeWidget(ui->comboBox_syncSensor);
    delete ui->comboBox_syncSensor;
#endif

    ui->labelStreamName->setText( streamName.c_str() );
    ui->labelSensorName->setText( sensorSpec.m_sensorName.c_str() );
    ui->labelResolutions->setText(
        hub::SensorSpec::resolutions2string( sensorSpec.m_resolutions ).c_str() );
    ui->labelSize->setText( ( std::to_string( sensorSpec.m_acquisitionSize ) + " bytes" ).c_str() );

    if ( sensorSpec.m_metaData.empty() ) {
        delete ui->label_metadata;
        delete ui->title_metadata;
        delete ui->line_metadata;
        delete ui->horizontalLayout_metaData;
        ui->verticalLayout_4->removeItem( ui->verticalLayout_metaData );
        delete ui->verticalLayout_metaData;
    }
    else {
        ui->label_metadata->setText(
            hub::SensorSpec::metaData2string( sensorSpec.m_metaData, true ).c_str() );
    }

#ifndef USE_COMPLETE_VIEWER
    mProxySensorModel.setSourceModel( &mSensorModel );
    QString sensorNameMod =
        QString( streamName.c_str() ).replace( "(", "\\(" ).replace( ")", "\\)" );
    mProxySensorModel.setFilterRegularExpression(
        QRegularExpression( QString( "^(?!" + sensorNameMod + "$)" ) ) );
    ui->comboBox_syncSensor->setModel( &mProxySensorModel );
#endif
}

FormStreamView::~FormStreamView() {

    delete ui;
}

#ifndef USE_COMPLETE_VIEWER
void FormStreamView::on_startStreaming() {

    ui->radioButtonOnOff->click();
}

void FormStreamView::on_radioButtonOnOff_clicked( bool checked ) {
    if ( checked ) {
        ui->radioButtonOnOff->setText( "on " );
        ui->frameButtonOnOff->setStyleSheet( "border-radius: 10px; background-color: lightgreen" );

        std::string syncSensorName = ui->comboBox_syncSensor->currentText().toStdString();
        if ( syncSensorName == "none" ) syncSensorName = "";

        ui->comboBox_syncSensor->setCurrentText( syncSensorName.c_str() );

        emit streamingStarted( m_streamName, syncSensorName );
        ui->comboBox_syncSensor->setEnabled( false );
    }
    else {
        ui->radioButtonOnOff->setText( "off" );
        ui->frameButtonOnOff->setStyleSheet( "border-radius: 10px; background-color: red" );
        emit streamingStopped( m_streamName, m_sensorSpec );
        ui->comboBox_syncSensor->setEnabled( true );
    }
}

// void FormStreamView::setRadioButtonOff() {
//     ui->radioButtonOnOff->setChecked( false );
//     ui->radioButtonOnOff->setText( "off" );
//     ui->frameButtonOnOff->setStyleSheet( "border-radius: 10px; background-color: red" );
// }

#endif
