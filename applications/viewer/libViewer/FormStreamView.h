#pragma once

#include <QMdiArea>
#include <QSortFilterProxyModel>
#include <QStringListModel>
#include <QThread>
#include <QWidget>

#include <MainWindowStreamView.h>

//#define USE_COMPLETE_VIEWER

namespace Ui {
class FormStreamView;
}


class FormStreamView : public QWidget
{
    Q_OBJECT

  public:
    FormStreamView( std::string streamName,
                    const hub::SensorSpec& sensorSpec,
                    QStringListModel& sensorModel,
                    QWidget* parent = nullptr );
    ~FormStreamView();

//    void setRadioButtonOff();
    bool isStarted() const;
    std::string  getSyncStreamName() const;


//#ifdef USE_COMPLETE_VIEWER
#ifndef USE_COMPLETE_VIEWER

signals:
    void startStream( const std::string& streamName, const hub::SensorSpec& sensorSpec, const std::string& syncStreamName );
    void stopStream( const std::string& streamName, const hub::SensorSpec& sensorSpec, const std::string& syncStreamName );

  public slots:
    void onStartStream();
    void on_radioButtonOnOff_clicked( bool checked );
//    void onStopStream();
#endif

  private:
    Ui::FormStreamView* ui;
#ifndef USE_COMPLETE_VIEWER
    const hub::SensorSpec m_sensorSpec;
    const std::string m_streamName;
    QStringListModel& mSensorModel;
    QSortFilterProxyModel mProxySensorModel;
#endif

public:
    const hub::SensorSpec &getSensorSpec() const;
};
