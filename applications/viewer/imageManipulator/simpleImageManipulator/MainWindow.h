#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <FormImageManipulator.h>
#include <WidgetStreamView.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow( QWidget* parent = nullptr );
    ~MainWindow();

  private:
    Ui::MainWindow* ui;

    FormImageManipulator* m_imageManipulator = nullptr;

    WidgetStreamView2D* m_streamView2D   = nullptr;
    WidgetStreamView2D* m_streamView2D_2 = nullptr;
};

#endif // MAINWINDOW_H
