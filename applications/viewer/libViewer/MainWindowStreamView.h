#pragma once

#include <QMainWindow>

#include <InputSensor.hpp>

namespace Ui {
class MainWindowStreamView;
}

class MainWindowStreamView : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindowStreamView( const hub::InputSensor& inputStream, QWidget* parent = nullptr );
    ~MainWindowStreamView();

    void setData( unsigned char* img_ptr, std::vector<int> dims, hub::Format format );

  signals:
    void onCloseStreamViewSignal();

  public slots:

  private:
    Ui::MainWindowStreamView* ui;

    const hub::InputSensor& m_inputStream;
};
