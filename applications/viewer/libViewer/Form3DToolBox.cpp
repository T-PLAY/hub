#include "Form3DToolBox.h"
#include "ui_Form3DToolBox.h"

Form3DToolBox::Form3DToolBox( QWidget* parent ) : QWidget( parent ), ui( new Ui::Form3DToolBox ) {
    ui->setupUi( this );

    m_comboBoxDisplayedTexture = ui->comboBox_displayedTexture;
}

Form3DToolBox::~Form3DToolBox() {
    delete ui;
}

void Form3DToolBox::on_pushButton_fitScene_clicked() {
    emit pushButton_fitScene_clicked();
}

void Form3DToolBox::on_pushButton_fitSelected_clicked() {
    emit pushButton_fitSelected_clicked();
}

void Form3DToolBox::on_pushButton_fitTrace_clicked() {
    emit pushButton_fitTrace_clicked();
}

void Form3DToolBox::on_checkBox_showGrid_toggled( bool checked ) {
    emit checkBox_showGrid_toggled( checked );
}

void Form3DToolBox::on_checkBox_showTrace_toggled( bool checked ) {
    emit checkBox_showTrace_toggled( checked );
}

void Form3DToolBox::on_doubleSpinBox_tune_valueChanged( double arg1 ) {
    emit doubleSpinBox_tune_valueChanged( arg1 );
}

void Form3DToolBox::on_doubleSpinBox_tune2_valueChanged( double arg1 ) {
    emit doubleSpinBox_tune2_valueChanged( arg1 );
}

void Form3DToolBox::on_doubleSpinBox_tune3_valueChanged( double arg1 ) {
    emit doubleSpinBox_tune3_valueChanged( arg1 );
}

void Form3DToolBox::on_doubleSpinBox_tune4_valueChanged( double arg1 ) {
    emit doubleSpinBox_tune4_valueChanged( arg1 );
}

void Form3DToolBox::on_pushButton_reloadShaders_clicked() {
    emit pushButton_reloadShaders_clicked();
}

void Form3DToolBox::on_comboBox_palettes_currentIndexChanged( int index ) {
    emit comboBox_palettes_currentIndexChanged( index );
}

void Form3DToolBox::on_checkBox_debug_toggled( bool checked ) {
    emit checkBox_debug_toggled( checked );
}

void Form3DToolBox::on_checkBox_transparency_toggled( bool checked ) {

    emit checkBox_transparency_toggled( checked );
}

void Form3DToolBox::on_horizontalSliderDynamic_valueChanged( int value ) {
    emit horizontalSliderDynamic_valueChanged( value );
}

void Form3DToolBox::on_checkBox_live_toggled( bool checked ) {
    emit checkBox_live_toggled( checked );
}
