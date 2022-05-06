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
            ClientSocket sock;
            sock.write( ClientSocket::Type::VIEWER );

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
                    std::string streamerSensorName;
                    sock.read( streamerSensorName );
                    std::string format;
                    sock.read( format );
                    std::string dims;
                    sock.read( dims );
                    std::string size;
                    sock.read( size );

                    Stream::MetaData metaData;
                    sock.read( metaData );

                    std::cout << "[Thread_Client] [viewer] new streamer " << streamerSensorName
                              << ", format:" << format << ", dims:" << dims
                              << ", acquisitionSize:" << size << std::endl;
                    std::cout << "[Thread_Client] [viewer] emit addSensorSignal '"
                              << streamerSensorName << "'" << std::endl;
                    std::cout << "[Thread_Client] [viewer] metadata : "
                              << Stream::to_string( metaData, true );
                    emit addSensorSignal( streamerSensorName,
                                          format,
                                          dims,
                                          size,
                                          Stream::to_string( metaData, true ) );

                } break;

                case Socket::Message::DEL_STREAMER: {
                    std::string streamerSensorName;
                    sock.read( streamerSensorName );
                    std::cout << "[Thread_Client] [viewer] del streamer '" << streamerSensorName
                              << "'" << std::endl;
                    emit delSensorSignal( streamerSensorName );
                } break;

                default:
                    std::cout << "[Thread_Client] unknown message from server" << std::endl;
                }
            }
        }
        catch ( std::exception& e ) {
            std::cout << "[Thread_Client] [viewer] catch exception : " << e.what() << std::endl;
        }
    } // while (!this->isInterruptionRequested())
}

SensorViews::SensorViews( QVBoxLayout& vboxLayout, QMdiArea& mdiArea, QObject* parent ) :
    QObject( parent ),
    mThreadClient( this ),
    m_vBoxLayout(vboxLayout),
    m_mdiArea(mdiArea)
{

    QObject::connect(
        &mThreadClient, &Thread_Client::addSensorSignal, this, &SensorViews::addSensor );
    QObject::connect(
        &mThreadClient, &Thread_Client::delSensorSignal, this, &SensorViews::delSensor );
    mThreadClient.start();
}


SensorViews::~SensorViews() {

    mThreadClient.requestInterruption();
    mThreadClient.wait();

    std::cout << "[SensorViews] ~SensorViews() mThreadClient.terminated()" << std::endl;

    for ( auto& pair : mStreamViews ) {
        auto* streamView = pair.second;

        assert( streamView != nullptr );
        std::cout << "[SensorViews] ~SensorViews() delete " << streamView << std::endl;
        delete streamView;
        streamView = nullptr;
    }
    mStreamViews.clear();
}

void SensorViews::addSensor( std::string streamerSensorName,
                             std::string format,
                             std::string dims,
                             std::string size,
                             std::string metaData ) {
    std::cout << "[SensorViews] SensorViews::addSensor '" << streamerSensorName << "'" << std::endl;

    FormSensorView* sensorView =
        new FormSensorView( nullptr, streamerSensorName, format, dims, size, metaData );
    m_vBoxLayout.insertWidget( static_cast<int>( mSensorViews.size() ), sensorView );

    mSensorViews[streamerSensorName] = sensorView;
    QObject::connect(
        sensorView, &FormSensorView::addViewStreamSignal, this, &SensorViews::addStreamView );
    QObject::connect(
        sensorView, &FormSensorView::delViewStreamSignal, this, &SensorViews::delStreamView );
}

void SensorViews::delSensor( std::string streamerSensorName ) {
    std::cout << "[SensorViews] SensorViews::delSensor '" << streamerSensorName << "'" << std::endl;

    delStreamView( streamerSensorName );

    assert( mSensorViews.find( streamerSensorName ) !=
            mSensorViews.end() ); // todo: fix one more time error
    FormSensorView* sensorView = mSensorViews.at( streamerSensorName );
    delete sensorView;
    mSensorViews.erase( streamerSensorName );

    std::cout << "[SensorViews] SensorViews::delSensor end '" << streamerSensorName << "'"
              << std::endl;
}

void SensorViews::addStreamView( std::string streamerSensorName ) {
    std::cout << "[SensorViews] SensorViews::addStreamView slot '" << streamerSensorName << "'"
              << ", nb streamView = " << mStreamViews.size() << std::endl;

    MainWindowStreamView* streamView = new MainWindowStreamView( &m_mdiArea, streamerSensorName );
    assert( mStreamViews.find( streamerSensorName ) == mStreamViews.end() );
    mStreamViews[streamerSensorName] = streamView;
    std::cout << "[SensorViews] SensorViews::addStreamView add " << streamView << std::endl;

    QMdiSubWindow* subWindow = m_mdiArea.addSubWindow( streamView );
    subWindow->setVisible( true );

    subWindow->setWindowTitle( streamerSensorName.c_str() );

    QObject::connect( streamView,
                      &MainWindowStreamView::onCloseStreamViewSignal,
                      this,
                      &SensorViews::onCloseStreamView );
}

#include <typeinfo>

void SensorViews::delStreamView( std::string streamerSensorName ) {
    std::cout << "[SensorViews] SensorViews::delStreamView slot, nb streamView = "
              << mStreamViews.size() << std::endl;

    if ( mStreamViews.find( streamerSensorName ) != mStreamViews.end() ) {
        MainWindowStreamView* streamView = mStreamViews.at( streamerSensorName );

        std::cout << "[SensorViews] SensorViews::delStreamView delete " << streamView << std::endl;

        m_mdiArea.removeSubWindow( streamView->parentWidget() );
        delete streamView;
        streamView = nullptr;
    }
    std::cout << "[SensorViews] SensorViews::delStreamView slot end, nb streamView = "
              << mStreamViews.size() << std::endl;
}

void SensorViews::onCloseStreamView( std::string streamerSensorName ) {
    std::cout << "[SensorViews] SensorViews::onCloseStreamView " << std::endl;
    assert( mSensorViews.find( streamerSensorName ) != mSensorViews.end() );
    mSensorViews.at( streamerSensorName )->setRadioButtonOff();

    assert( mStreamViews.find( streamerSensorName ) != mStreamViews.end() );
    mStreamViews.erase( streamerSensorName );
}
