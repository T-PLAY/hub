#pragma once

#include <QMdiArea>
#include <QSortFilterProxyModel>
#include <QStringListModel>
#include <QThread>
#include <QWidget>

#include <MainWindowStreamView.h>

#define USE_COMPLETE_VIEWER

namespace Ui {
class FormStreamView;
}

class FormStreamView : public QWidget
{
    Q_OBJECT

  public:
    FormStreamView( std::string streamName,
                    const hub::SensorSpec& sensorSpec,
                    const std::string & syncStreamName,
                    QStringListModel& sensorModel,
                    QWidget* parent = nullptr );
    ~FormStreamView();

//    void setRadioButtonOff();

//#ifdef USE_COMPLETE_VIEWER
#ifndef USE_COMPLETE_VIEWER
  signals:
    void streamingStarted( const std::string& streamName, const std::string& syncSensorName );
    void streamingStopped( const std::string& streamName, const hub::SensorSpec& sensorSpec );

  public slots:
    void on_startStreaming();
    void on_radioButtonOnOff_clicked( bool checked );
#endif

  private:
    Ui::FormStreamView* ui;
#ifndef USE_COMPLETE_VIEWER
    const hub::SensorSpec m_sensorSpec;
    const std::string m_streamName;
    QStringListModel& mSensorModel;
    QSortFilterProxyModel mProxySensorModel;
#endif
};
