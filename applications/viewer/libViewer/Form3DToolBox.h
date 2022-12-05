#ifndef FORM3DTOOLBOX_H
#define FORM3DTOOLBOX_H

#include <QComboBox>
#include <QWidget>

namespace Ui {
class Form3DToolBox;
}

class Form3DToolBox : public QWidget
{
    Q_OBJECT

  public:
    explicit Form3DToolBox( QWidget* parent = nullptr );
    ~Form3DToolBox();

  signals:
    void pushButton_fitScene_clicked();
    void pushButton_fitSelected_clicked();
    void pushButton_fitTrace_clicked();

    void checkBox_showGrid_toggled( bool checked );
    void checkBox_showTrace_toggled( bool checked );
    void pushButton_reloadShaders_clicked();
    void doubleSpinBox_tune3_valueChanged( double arg1 );
    void doubleSpinBox_tune4_valueChanged( double arg1 );
    void doubleSpinBox_tune_valueChanged( double arg1 );
    void doubleSpinBox_tune2_valueChanged( double arg1 );
    void comboBox_palettes_currentIndexChanged( int index );
    void checkBox_debug_toggled( bool checked );
    void checkBox_transparency_toggled( bool checked );
    void horizontalSliderDynamic_valueChanged( int value );
    void checkBox_live_toggled( bool checked );

    void record_start();
    void record_stop();

  public slots:
    void on_pushButton_fitScene_clicked();

    void on_checkBox_showGrid_toggled( bool checked );
    void on_checkBox_showTrace_toggled( bool checked );

  private slots:
    void on_pushButton_reloadShaders_clicked();
    void on_doubleSpinBox_tune_valueChanged( double arg1 );
    void on_doubleSpinBox_tune2_valueChanged( double arg1 );
    void on_doubleSpinBox_tune3_valueChanged( double arg1 );
    void on_doubleSpinBox_tune4_valueChanged( double arg1 );

    void on_comboBox_palettes_currentIndexChanged( int index );

    void on_checkBox_debug_toggled( bool checked );

    void on_checkBox_transparency_toggled( bool checked );

  public:
    QComboBox* m_comboBoxDisplayedTexture = nullptr;

  private slots:

    void on_checkBox_live_toggled( bool checked );

    void on_pushButton_record_clicked();

private:
    Ui::Form3DToolBox* ui;
};

#endif // FORM3DTOOLBOX_H
