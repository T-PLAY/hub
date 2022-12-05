#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <IO/Stream.hpp>
#include <OutputSensor.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow( QWidget* parent = nullptr );
    ~MainWindow();

  private slots:

  private:
    void updateTransform();

  private:
    Ui::MainWindow* ui;

    hub::OutputSensor* m_outputSensor = nullptr;
    glm::mat4 m_transform;
};

#endif // MAINWINDOW_H
