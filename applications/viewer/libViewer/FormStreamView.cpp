#include "FormStreamView.h"
#include "ui_FormStreamView.h"

#include <QMdiSubWindow>
#include <WidgetStreamView.h>
#include <cmath>
#include <iostream>
#include <constants.h>

// Thread_InputStream::Thread_InputStream( std::string streamName, QObject* parent ) :
//     QThread( parent ), mInputStream( ClientSocket( streamName, "" ) ) {
//     std::cout << "Thread_InputStream()" << std::endl;
// }

// Thread_InputStream::~Thread_InputStream() {
//     std::cout << "~Thread_InputStream()" << std::endl;
// }

// void Thread_InputStream::run() {
//     std::cout << "Thread_InputStream::run()" << std::endl;

//    try {

//        while ( !this->isInterruptionRequested() ) {

//            mInputStream >> mAcq;
//            // std::cout << "receive acq : " << mAcq << std::endl;

//            emit newAcquisition();
//        }
//    }
//    catch ( std::exception& e ) {
//        std::cout << "[streamView] catch exception : " << e.what() << std::endl;
////        emit streamingStopped(mInputStream.getSensorName());
//        return;
//    }

//    std::cout << "Thread_InputStream::run() end" << std::endl;
//}

///////////////////////////////////////////////////////////////////////////////////

FormStreamView::FormStreamView(std::string streamName,
    const hub::SensorSpec &sensorSpec,
    QStringListModel& sensorModel,
    QWidget* parent)

    : QWidget(parent)
    , ui(new Ui::FormStreamView)
    , m_streamName(streamName)
    , mSensorModel(sensorModel)
{
    //    m_mdiArea( mdiArea ) {
    ui->setupUi(this);

    ui->labelStreamName->setText(streamName.c_str());
    ui->labelSensorName->setText(sensorSpec.m_sensorName.c_str());
    ui->labelFormat->setText(hub::SensorSpec::format2string(sensorSpec.m_format).c_str());
    ui->laberDimensions->setText(hub::SensorSpec::dims2string(sensorSpec.m_dims).c_str());
    ui->labelSize->setText((std::to_string(sensorSpec.m_acquisitionSize) + " bytes").c_str());

    if (sensorSpec.m_metaData.empty()) {
        delete ui->label_metadata;
        delete ui->title_metadata;
        delete ui->line_metadata;
        delete ui->horizontalLayout_metaData;
        ui->verticalLayout_4->removeItem(ui->verticalLayout_metaData);
        delete ui->verticalLayout_metaData;
    } else {
        ui->label_metadata->setText(hub::SensorSpec::metaData2string(sensorSpec.m_metaData).c_str());
    }

    mProxySensorModel.setSourceModel(&mSensorModel);
    //    mProxySensorModel.setFilterFixedString(streamName.c_str());
    //    mProxySensorModel.setFilterWildcard(streamName.c_str());
    //    mProxySensorModel.setFilterRegularExpression(QRegularExpression(("^[" + streamName + "]").c_str()));
    //    mProxySensorModel.setFilterRegularExpression(QRegularExpression(QString("^(?!\b") + streamName.c_str() + "\b)"));
    QString sensorNameMod = QString(streamName.c_str()).replace("(", "\\(").replace(")", "\\)");
    //    mProxySensorModel.setFilterRegularExpression(QRegularExpression(QString("^(?!Polhemus Patriot \\(probe\\))")));
    mProxySensorModel.setFilterRegularExpression(QRegularExpression(QString("^(?!" + sensorNameMod + ")")));
    ui->comboBox_syncSensor->setModel(&mProxySensorModel);
    //    ui->comboBox_syncSensor;

    //    ui->label_frequency->setText( "0 Hz" );

    //    on_radioButtonOnOff_clicked(true);
//    ui->radioButtonOnOff->click();
}

FormStreamView::~FormStreamView()
{
    //    on_stopStreaming();
    //    if (m_inputStreamThread != nullptr) {
    //        assert(m_streamView != nullptr);

    //        delete m_inputStreamThread;
    //        m_inputStreamThread = nullptr;
    //        delete m_streamView;
    //        m_streamView = nullptr;
    //    }
    delete ui;
}

void FormStreamView::on_radioButtonOnOff_clicked(bool checked)
{
    if (checked) {
        ui->radioButtonOnOff->setText("on ");
        ui->frameButtonOnOff->setStyleSheet("border-radius: 10px; background-color: lightgreen");
        //        emit addViewStreamSignal(m_streamName);
        //        on_startStreamingPrivate();

        std::string syncSensorName = ui->comboBox_syncSensor->currentText().toStdString();
        if (syncSensorName == "none")
            syncSensorName = "";

//        if (syncSensorName == "") {
//            if (m_streamName == g_probePoseSensorName) {
////                if (mSensorModel.stringList().contains(g_probeScanSensorName.c_str())) {
//                    syncSensorName = g_probeScanSensorName;
////                }
//            }
//        }
        ui->comboBox_syncSensor->setCurrentText(syncSensorName.c_str());

        emit streamingStarted(m_streamName, syncSensorName);
        ui->comboBox_syncSensor->setEnabled(false);

    } else {
        ui->radioButtonOnOff->setText("off");
        ui->frameButtonOnOff->setStyleSheet("border-radius: 10px; background-color: red");
        //        emit delViewStreamSignal(m_streamName);
        //        on_stopStreaming();
        emit streamingStopped(m_streamName);
        ui->comboBox_syncSensor->setEnabled(true);
    }
}

void FormStreamView::setRadioButtonOff()
{
    ui->radioButtonOnOff->setChecked(false);
    ui->radioButtonOnOff->setText("off");
    ui->frameButtonOnOff->setStyleSheet("border-radius: 10px; background-color: red");
}

void FormStreamView::on_startStreaming()
{
    //    on_radioButtonOnOff_clicked(true);
    //    ui->radioButtonOnOff->clicked(true);
    //    ui->radioButtonOnOff->setChecked(true);

        ui->radioButtonOnOff->click();
}

// void FormStreamView::on_startStreamingPrivate() {
//     std::cout << "[FormStreamView] FormStreamView::on_startStreamingPrivate slot '" << m_streamName << "'"
//               << std::endl;
//     //              << ", nb streamView = " << mStreamViews.size() << std::endl;

//    assert( m_inputStreamThread == nullptr );
//    m_inputStreamThread = new Thread_InputStream( m_streamName, this );
//    QObject::connect(
//        m_inputStreamThread, &Thread_InputStream::newAcquisition, this, &FormStreamView::on_newAcquisition );
////    QObject::connect(
////        m_inputStreamThread, &Thread_InputStream::streamingStopped, this, &FormStreamView::streamingStopped );

//    assert( m_streamView == nullptr );
//    m_streamView = new MainWindowStreamView( m_inputStreamThread->mInputStream, this );

//    //    assert( mStreamViews.find( m_streamName ) == mStreamViews.end() );
//    //    mStreamViews[m_streamName] = streamView;
//    //    std::cout << "[FormStreamView] FormStreamView::on_startStreamingPrivate " << m_streamName <<
//    //    std::endl;

////    QMdiSubWindow* subWindow = m_mdiArea.addSubWindow( m_streamView );
////    subWindow->setVisible( true );
////    subWindow->setWindowTitle( m_streamName.c_str() );

//    QObject::connect( m_streamView,
//                      &MainWindowStreamView::onCloseStreamViewSignal,
//                      this,
//                      &FormStreamView::on_closeStreamView );

//    mCounterFps = 0;
//    mFps        = 10;
//    mStartFps   = std::chrono::high_resolution_clock::now();
//    m_inputStreamThread->start();

//    emit streamingStarted(m_streamName);
//}

//#include <typeinfo>

// void FormStreamView::on_stopStreaming() {
//     std::cout << "[FormStreamView] FormStreamView::on_stopStreaming " << m_streamName << std::endl;
//     //              << mStreamViews.size() << std::endl;

//        if (m_inputStreamThread == nullptr)
//            return;

//    assert( m_inputStreamThread != nullptr );
//    //    if ( mStreamViews.find( streamName ) != mStreamViews.end() ) {
//    //        MainWindowStreamView* streamView = mStreamViews.at( streamName );

//    //        std::cout << "[FormStreamView] FormStreamView::on_stopStreaming delete " << streamView <<
//    //        std::endl;
//    m_inputStreamThread->requestInterruption();
//    m_inputStreamThread->wait();
//    delete m_inputStreamThread;
//    m_inputStreamThread = nullptr;
//    ui->label_frequency->setText( "0 Hz" );

//    //    assert( m_streamView != nullptr );
////    if ( m_streamView != nullptr ) {
////        m_mdiArea.removeSubWindow( m_streamView->parentWidget() );
////        delete m_streamView;
////        m_streamView = nullptr;
////    }
//    //    }
//    //    std::cout << "[FormStreamView] FormStreamView::on_stopStreaming slot end, nb streamView = "
//    //              << mStreamViews.size() << std::endl;

//    emit streamingStopped(m_streamName);
//}

// void FormStreamView::on_closeStreamView() {

//        m_mdiArea.removeSubWindow( m_streamView->parentWidget() );
////        delete m_streamView;
//        m_streamView = nullptr;
//        setRadioButtonOff();
//    on_stopStreaming();

//    //    std::cout << "[FormStreamView] FormStreamView::on_closeStreamView " << std::endl;
//    //    assert( mSensorViews.find( streamName ) != mSensorViews.end() );
//    //    mSensorViews.at( streamName )->setRadioButtonOff();

//    //    assert( mStreamViews.find( streamName ) != mStreamViews.end() );
//    //    mStreamViews.erase( streamName );
//}

// void FormStreamView::on_newAcquisition() {
//     //    std::cout << "[FormStreamView] on_newAcquisition" << std::endl;
//     if ( m_inputStreamThread == nullptr ) return;

//    if ( mCounterFps == std::ceil( mFps ) ) {
//        const auto end = std::chrono::high_resolution_clock::now();
//        const auto duration =
//            std::chrono::duration_cast<std::chrono::microseconds>( end - mStartFps ).count();
//        mFps = ( 1'000'000.0 * std::ceil( mFps ) ) / duration;
//        //        ui->statusbar->showMessage((std::string("fps:") + std::to_string(mFps)).c_str());
//        mCounterFps = 0;
//        mStartFps   = std::chrono::high_resolution_clock::now();
//        ui->label_frequency->setText( ( std::to_string( mFps ) + " Hz" ).c_str() );
//    }

//    assert( m_inputStreamThread != nullptr );
//    assert( m_inputStreamThread->mAcq.mData != nullptr );
//    m_streamView->setData( (unsigned char*)m_inputStreamThread->mAcq.mData,
//                           m_inputStreamThread->mInputStream.getDims(),
//                           m_inputStreamThread->mInputStream.getFormat() );
//    //        (static_cast<WidgetStreamView*>(m_streamView->ui->centralwidget))->setData((unsigned
//    //    char*)mThread.mAcq.mData, mThread.mInputStream.getDims(),
//    //    mThread.mInputStream.getFormat());

//    ++mCounterFps;

//    emit newAcquisition();
//}

// const InputStream &FormStreamView::getInputStream() const
//{
//     assert(m_inputStreamThread != nullptr);
//     return m_inputStreamThread->mInputStream;
// }

// const Thread_InputStream *FormStreamView::getInputStreamThread() const
//{
//     assert(m_inputStreamThread != nullptr);
//     return m_inputStreamThread;
// }

//void FormStreamView::on_comboBox_syncSensor_currentTextChanged(const QString& syncSensorName)
//{
//    std::cout << "[FormStreamView] FormStreamView::on_comboBox_syncSensor_currentTextChanged " << syncSensorName.toStdString() << std::endl;
//    if (ui->radioButtonOnOff->isEnabled()) {
//        emit streamingStopped(m_streamName);
//        emit streamingStarted(m_streamName, (syncSensorName == "none") ? "" : syncSensorName.toStdString());
//    }
//}
