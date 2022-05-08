#include "FormSensorView.h"
#include "ui_FormSensorView.h"

#include <QMdiSubWindow>
#include <WidgetStreamView.h>
#include <iostream>
#include <stream.h>

Thread_InputStream::Thread_InputStream( std::string sensorName, QObject* parent ) :
    QThread( parent ), mInputStream( ClientSocket( sensorName, "" ) ) {
    std::cout << "Thread_InputStream()" << std::endl;
}

Thread_InputStream::~Thread_InputStream() {
    std::cout << "~Thread_InputStream()" << std::endl;
}

void Thread_InputStream::run() {
    std::cout << "Thread_InputStream::run()" << std::endl;

    try {

        while ( !this->isInterruptionRequested() ) {

            mInputStream >> mAcq;
            // std::cout << "receive acq : " << mAcq << std::endl;

            emit newImage();
        }
    }
    catch ( std::exception& e ) {
        std::cout << "[streamView] catch exception : " << e.what() << std::endl;
        return;
    }

    std::cout << "Thread_InputStream::run() end" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////////

FormSensorView::FormSensorView( std::string sensorName,
                                std::string format,
                                std::string dims,
                                std::string size,
                                std::string metaData,
                                QMdiArea& mdiArea,
                                QWidget* parent ) :
    QWidget( parent ),
    ui( new Ui::FormSensorView ),
    mSensorName( sensorName ),
    m_mdiArea( mdiArea ) {
    ui->setupUi( this );

    ui->labelSensorName->setText( sensorName.c_str() );
    ui->labelFormat->setText( format.c_str() );
    ui->laberDimensions->setText( dims.c_str() );
    ui->labelSize->setText( ( size + " bytes" ).c_str() );

    if ( metaData.empty() ) {
        delete ui->label_metadata;
        delete ui->title_metadata;
        delete ui->line_metadata;
        delete ui->horizontalLayout_metaData;
        ui->verticalLayout_4->removeItem( ui->verticalLayout_metaData );
        delete ui->verticalLayout_metaData;
    }
    else {
        ui->label_metadata->setText( metaData.c_str() );
    }

    ui->label_frequency->setText( "0 Hz" );
}

FormSensorView::~FormSensorView() {
    delete ui;
}

void FormSensorView::on_radioButtonOnOff_clicked( bool checked ) {
    if ( checked ) {
        ui->radioButtonOnOff->setText( "on " );
        ui->frameButtonOnOff->setStyleSheet( "border-radius: 10px; background-color: lightgreen" );
        //        emit addViewStreamSignal(mSensorName);
        startStreaming();
    }
    else {
        ui->radioButtonOnOff->setText( "off" );
        ui->frameButtonOnOff->setStyleSheet( "border-radius: 10px; background-color: red" );
        //        emit delViewStreamSignal(mSensorName);
        stopStreaming();
    }
}

void FormSensorView::setRadioButtonOff() {
    ui->radioButtonOnOff->setChecked( false );
    ui->radioButtonOnOff->setText( "off" );
    ui->frameButtonOnOff->setStyleSheet( "border-radius: 10px; background-color: red" );
}

void FormSensorView::startStreaming() {
    std::cout << "[FormSensorView] FormSensorView::startStreaming slot '" << mSensorName << "'"
              << std::endl;
    //              << ", nb streamView = " << mStreamViews.size() << std::endl;

    assert( m_inputStreamThread == nullptr );
    m_inputStreamThread = new Thread_InputStream( mSensorName, this );
    QObject::connect(
        m_inputStreamThread, &Thread_InputStream::newImage, this, &FormSensorView::newImage );

    assert( m_streamView == nullptr );
    m_streamView = new MainWindowStreamView( m_inputStreamThread->mInputStream, this );

    //    assert( mStreamViews.find( mSensorName ) == mStreamViews.end() );
    //    mStreamViews[mSensorName] = streamView;
    //    std::cout << "[FormSensorView] FormSensorView::startStreaming " << mSensorName <<
    //    std::endl;

    QMdiSubWindow* subWindow = m_mdiArea.addSubWindow( m_streamView );
    subWindow->setVisible( true );
    subWindow->setWindowTitle( mSensorName.c_str() );

    QObject::connect( m_streamView,
                      &MainWindowStreamView::onCloseStreamViewSignal,
                      this,
                      &FormSensorView::onCloseStreamView );

    mCounterFps = 0;
    mFps        = 10;
    mStartFps   = std::chrono::high_resolution_clock::now();
    m_inputStreamThread->start();
}

//#include <typeinfo>

void FormSensorView::stopStreaming() {
    std::cout << "[FormSensorView] FormSensorView::stopStreaming " << mSensorName << std::endl;
    //              << mStreamViews.size() << std::endl;
        if (m_inputStreamThread == nullptr)
            return;

    assert( m_inputStreamThread != nullptr );
    //    if ( mStreamViews.find( sensorName ) != mStreamViews.end() ) {
    //        MainWindowStreamView* streamView = mStreamViews.at( sensorName );

    //        std::cout << "[FormSensorView] FormSensorView::stopStreaming delete " << streamView <<
    //        std::endl;
    m_inputStreamThread->requestInterruption();
    m_inputStreamThread->wait();
    delete m_inputStreamThread;
    m_inputStreamThread = nullptr;
    ui->label_frequency->setText( "0 Hz" );

    //    assert( m_streamView != nullptr );
    if ( m_streamView != nullptr ) {
        m_mdiArea.removeSubWindow( m_streamView->parentWidget() );
        delete m_streamView;
        m_streamView = nullptr;
    }
    //    }
    //    std::cout << "[FormSensorView] FormSensorView::stopStreaming slot end, nb streamView = "
    //              << mStreamViews.size() << std::endl;
}

void FormSensorView::onCloseStreamView() {

        m_mdiArea.removeSubWindow( m_streamView->parentWidget() );
//        delete m_streamView;
        m_streamView = nullptr;
        setRadioButtonOff();
    stopStreaming();

    //    std::cout << "[FormSensorView] FormSensorView::onCloseStreamView " << std::endl;
    //    assert( mSensorViews.find( sensorName ) != mSensorViews.end() );
    //    mSensorViews.at( sensorName )->setRadioButtonOff();

    //    assert( mStreamViews.find( sensorName ) != mStreamViews.end() );
    //    mStreamViews.erase( sensorName );
}

void FormSensorView::newImage() {
    //    std::cout << "[FormSensorView] newImage" << std::endl;
    if ( m_inputStreamThread == nullptr ) return;

    if ( mCounterFps == std::ceil( mFps ) ) {
        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>( end - mStartFps ).count();
        mFps = ( 1'000'000.0 * std::ceil( mFps ) ) / duration;
        //        ui->statusbar->showMessage((std::string("fps:") + std::to_string(mFps)).c_str());
        mCounterFps = 0;
        mStartFps   = std::chrono::high_resolution_clock::now();
        ui->label_frequency->setText( ( std::to_string( mFps ) + " Hz" ).c_str() );
    }

    assert( m_inputStreamThread != nullptr );
    assert( m_inputStreamThread->mAcq.mData != nullptr );
    m_streamView->setData( (unsigned char*)m_inputStreamThread->mAcq.mData,
                           m_inputStreamThread->mInputStream.getDims(),
                           m_inputStreamThread->mInputStream.getFormat() );
    //        (static_cast<WidgetStreamView*>(m_streamView->ui->centralwidget))->setData((unsigned
    //    char*)mThread.mAcq.mData, mThread.mInputStream.getDims(),
    //    mThread.mInputStream.getFormat());

    ++mCounterFps;
}
