#ifndef FORM3DTOOLBOX_H
#define FORM3DTOOLBOX_H

#include <QWidget>
#include <QComboBox>

namespace Ui {
class Form3DToolBox;
}

class Form3DToolBox : public QWidget
{
    Q_OBJECT

public:
    explicit Form3DToolBox(QWidget *parent = nullptr);
    ~Form3DToolBox();

signals:
    void pushButton_fitScene_clicked();
    void pushButton_fitTrace_clicked();

    void checkBox_showGrid_toggled(bool checked);
    void checkBox_showTrace_toggled(bool checked);
    void doubleSpinBox_transparency_valueChanged(double transparency);
    void doubleSpinBox_transparency2_valueChanged(double transparency);
    void pushButton_reloadShaders_clicked();

public slots:
    void on_pushButton_fitScene_clicked();
    void on_pushButton_fitTrace_clicked();

    void on_checkBox_showGrid_toggled(bool checked);
    void on_checkBox_showTrace_toggled(bool checked);

private slots:
    void on_doubleSpinBox_transparency_valueChanged(double transparency);

    void on_doubleSpinBox_transparency_2_valueChanged(double transparency);

    void on_pushButton_reloadShaders_clicked();

public:
    QComboBox * m_comboBoxDisplayedTexture = nullptr;

private:
    Ui::Form3DToolBox *ui;

};

#endif // FORM3DTOOLBOX_H
