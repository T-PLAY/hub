#include "FormStreamView.h"
#include "ui_FormStreamView.h"

#include <QMdiSubWindow>
#include <WidgetStreamView.h>
#include <cmath>
#include <constants.h>
#include <iostream>

// Thread_InputStream::Thread_InputStream( std::string streamName, QObject* parent ) :
// }

// Thread_InputStream::~Thread_InputStream() {
// }

// void Thread_InputStream::run() {

//}

///////////////////////////////////////////////////////////////////////////////////

FormStreamView::FormStreamView( std::string streamName,
                                const hub::SensorSpec& sensorSpec,
                                QStringListModel& sensorModel,
                                QWidget* parent )

    :
    QWidget( parent ),
    ui( new Ui::FormStreamView ),
    m_streamName( streamName ),
    m_sensorSpec( sensorSpec ),
    mSensorModel( sensorModel ) {
    ui->setupUi( this );

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

    mProxySensorModel.setSourceModel( &mSensorModel );
    QString sensorNameMod =
        QString( streamName.c_str() ).replace( "(", "\\(" ).replace( ")", "\\)" );
    mProxySensorModel.setFilterRegularExpression(
        QRegularExpression( QString( "^(?!" + sensorNameMod + "$)" ) ) );
    ui->comboBox_syncSensor->setModel( &mProxySensorModel );
}

FormStreamView::~FormStreamView() {

    delete ui;
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

void FormStreamView::setRadioButtonOff() {
    ui->radioButtonOnOff->setChecked( false );
    ui->radioButtonOnOff->setText( "off" );
    ui->frameButtonOnOff->setStyleSheet( "border-radius: 10px; background-color: red" );
}

void FormStreamView::on_startStreaming() {

    ui->radioButtonOnOff->click();
}

// void FormStreamView::on_startStreamingPrivate() {

///&FormStreamView::streamingStopped );

//}

//#include <typeinfo>

// void FormStreamView::on_stopStreaming() {

//}

// void FormStreamView::on_closeStreamView() {

//}

// void FormStreamView::on_newAcquisition() {

//}

// const InputStream &FormStreamView::getInputStream() const
//{
// }

// const Thread_InputStream *FormStreamView::getInputStreamThread() const
//{
// }

// void FormStreamView::on_comboBox_syncSensor_currentTextChanged(const QString& syncSensorName)
//{
// }
