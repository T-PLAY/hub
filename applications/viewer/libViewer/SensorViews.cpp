#include "SensorViews.h"

#include <FormSensorView.h>
#include <QMdiSubWindow>
//#include <QThread>

Thread_Client::Thread_Client( QObject* parent ) : QThread( parent ) {
    std::cout << "[Thread_Client] Thread_Client()" << std::endl;
}

void Thread_Client::run() {
    std::cout << "[Thread_Client] Thread_Client::run()" << std::endl;

    while ( !this->isInterruptionRequested() ) {
        try {
            ClientSocket sock(m_dialog.getIpV4(), m_dialog.getPort());
            sock.write( ClientSocket::Type::VIEWER );
            emit serverConnected();
            m_dialog.hide();

            while ( !this->isInterruptionRequested() ) {

                Socket::Message serverMessage;
                sock.read( serverMessage );

                switch ( serverMessage ) {

                case Socket::Message::PING: {
                    // server check client connection
                    // nothing to do
                } break;

                case Socket::Message::NEW_STREAMER: {
                    std::cout << "[Thread_Client] [viewer] new streamer" << std::endl;
                    std::string sensorName;
                    sock.read( sensorName );
                    std::string format;
                    sock.read( format );
                    std::string dims;
                    sock.read( dims );
                    std::string size;
                    sock.read( size );

                    Stream::MetaData metaData;
                    sock.read( metaData );

                    std::cout << "[Thread_Client] [viewer] new streamer " << sensorName
                              << ", format:" << format << ", dims:" << dims
                              << ", acquisitionSize:" << size << std::endl;
                    std::cout << "[Thread_Client] [viewer] emit addSensorSignal '" << sensorName
                              << "'" << std::endl;
                    std::cout << "[Thread_Client] [viewer] metadata : "
                              << Stream::to_string( metaData, true );
                    emit addSensorSignal(
                        sensorName, format, dims, size, Stream::to_string( metaData, true ) );

                    //                    InputStream inputStream(sensorName.c_str(), "");

                } break;

                case Socket::Message::DEL_STREAMER: {
                    std::string sensorName;
                    sock.read( sensorName );
                    std::cout << "[Thread_Client] [viewer] del streamer '" << sensorName << "'"
                              << std::endl;
                    emit delSensorSignal( sensorName );
                } break;

                default:
                    std::cout << "[Thread_Client] unknown message from server" << std::endl;
                }
            }
        }
        catch ( std::exception& e ) {
            emit serverDisconnected();
            m_dialog.show();
            std::cout << "[Thread_Client] [viewer] no server, catch exception : " << e.what()
                      << std::endl;
        }
    } // while (!this->isInterruptionRequested())
}

///////////////////////////////////////////////////////////////////////////////////////

SensorViews::SensorViews( QBoxLayout& vboxLayout,
                          QMdiArea& mdiArea,
                          QMainWindow& mainWindow,
                          QObject* parent ) :
    QObject( parent ),
    mThreadClient( this ),
    m_vBoxLayout( vboxLayout ),
    m_mdiArea( mdiArea ),
    m_mainWindow( mainWindow ) {

    QObject::connect(
        &mThreadClient, &Thread_Client::addSensorSignal, this, &SensorViews::addSensor );
    QObject::connect(
        &mThreadClient, &Thread_Client::delSensorSignal, this, &SensorViews::delSensor );

    QObject::connect( &mThreadClient, &Thread_Client::serverConnected, this, &SensorViews::onServerConnect);
    QObject::connect( &mThreadClient, &Thread_Client::serverDisconnected, this, &SensorViews::onServerDisconnect);
//    QObject::connect( &mThreadClient, &Thread_Client::serverConnected, this, [this]() {
//        m_mainWindow.setEnabled( true );
//    } );
//    QObject::connect( &mThreadClient, &Thread_Client::serverDisconnected, this, [this]() {
//        m_mainWindow.setEnabled( false );
//    } );
//    m_mainWindow.setEnabled(false);
    mThreadClient.start();
}

SensorViews::~SensorViews() {

    mThreadClient.requestInterruption();
    mThreadClient.wait();

    std::cout << "[SensorViews] ~SensorViews() mThreadClient.terminated()" << std::endl;

    //    for ( auto& pair : mStreamViews ) {
    //        auto* streamView = pair.second;

    //        assert( streamView != nullptr );
    //        std::cout << "[SensorViews] ~SensorViews() delete " << streamView << std::endl;
    //        delete streamView;
    //        streamView = nullptr;
    //    }
    //    mStreamViews.clear();
    for ( auto& pair : m_sensorViews ) {
        auto* sensorView = pair.second;
        delete sensorView;
    }
}

void SensorViews::onServerConnect() {
    m_mainWindow.setEnabled(true);
//    delete m_dialog;
//    m_dialog = nullptr;
//    m_dialog->hide();
}

void SensorViews::onServerDisconnect() {
    m_mainWindow.setEnabled(false);
//    m_dialog = new DialogServerConnect(&m_mainWindow);
//    m_dialog->show();
}

void SensorViews::addSensor( std::string sensorName,
                             std::string format,
                             std::string dims,
                             std::string size,
                             std::string metaData ) {
    std::cout << "[SensorViews] SensorViews::addSensor '" << sensorName << "'" << std::endl;
    assert( m_sensorViews.find( sensorName ) == m_sensorViews.end() );

    auto* sensorView =
        new FormSensorView( sensorName, format, dims, size, metaData, m_mdiArea, nullptr );
    m_vBoxLayout.insertWidget( static_cast<int>( m_sensorViews.size() ), sensorView );

    m_sensorViews[sensorName] = sensorView;
    //    QObject::connect(
    //        sensorView, &FormSensorView::addViewStreamSignal, this, &SensorViews::addStreamView );
    //    QObject::connect(
    //            sensorView, &FormSensorView::delViewStreamSignal, this,
    //            &SensorViews::delStreamView );
    //    emit sensorAdded(sensorName);
    QObject::connect(
        sensorView, &FormSensorView::streamingStarted, this, &SensorViews::streamingStarted );
    QObject::connect(
        sensorView, &FormSensorView::streamingStopped, this, &SensorViews::streamingStopped );
}

void SensorViews::delSensor( std::string sensorName ) {
    std::cout << "[SensorViews] SensorViews::delSensor '" << sensorName << "'" << std::endl;

    //    delStreamView( sensorName );

    assert( m_sensorViews.find( sensorName ) != m_sensorViews.end() );
    auto* sensorView = m_sensorViews.at( sensorName );
    m_sensorViews.erase( sensorName );
    delete sensorView;

    //    assert( mSensorViews.find( sensorName ) !=
    //            mSensorViews.end() ); // todo: fix one more time error
    //    FormSensorView* sensorView = mSensorViews.at( sensorName );
    //    delete sensorView;
    //    mSensorViews.erase( sensorName );

    std::cout << "[SensorViews] SensorViews::delSensor end '" << sensorName << "'" << std::endl;

    //    emit sensorDeleted(sensorName);
}

const FormSensorView& SensorViews::getSensorView( const std::string& sensorName ) const {
    assert( m_sensorViews.find( sensorName ) != m_sensorViews.end() );
    return *m_sensorViews.at( sensorName );
}
