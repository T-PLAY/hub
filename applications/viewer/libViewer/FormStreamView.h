#ifndef FORMSENSORVIEW_H
#define FORMSENSORVIEW_H

#include <MainWindowStreamView.h>
#include <QMdiArea>
#include <QThread>
#include <QWidget>

#include <QSortFilterProxyModel>
//#include <FormInputStreamViews.h>

// class Thread_InputStream : public QThread
//{




//};

#include <QStringListModel>

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

    void setRadioButtonOff();

  signals:
    void streamingStarted( const std::string& streamName, const std::string& syncSensorName );
    void streamingStopped( const std::string& streamName, const hub::SensorSpec& sensorSpec );

  public slots:
    void on_startStreaming();
    void on_radioButtonOnOff_clicked( bool checked );

  private slots:



  public:


    const hub::SensorSpec m_sensorSpec;

  private:
    Ui::FormStreamView* ui;
    const std::string m_streamName;
    QStringListModel& mSensorModel;
    QSortFilterProxyModel mProxySensorModel;

};

#endif // FORMSENSORVIEW_H
