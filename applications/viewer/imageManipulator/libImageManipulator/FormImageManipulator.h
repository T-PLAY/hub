#pragma once

#include <QWidget>
#include <WidgetStreamView.h>

#include <Acquisition.hpp>

namespace Ui {
class FormImageManipulator;
}

class FormImageManipulator : public QWidget
{
    Q_OBJECT

  public:
    explicit FormImageManipulator( QWidget* parent = nullptr );
    ~FormImageManipulator();

    void init();

  public slots:
    void onStreamViewInited();

  public:
    WidgetStreamView2D& getWidgetStreamView();

  protected:
  private:
    Ui::FormImageManipulator* ui;
};
