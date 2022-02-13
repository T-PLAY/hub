#ifndef QWIDGET_FRAME_H
#define QWIDGET_FRAME_H

#include <QObject>
#include <QWidget>

class QWidget_Frame : public QWidget
{
    Q_OBJECT

public:
    explicit QWidget_Frame(QWidget *parent = 0);

public slots:
    void setImage(unsigned char *img_ptr, int width, int height);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
//    QImage image;
//    uchar * imagePtr = nullptr;
//    static const int imgSize = 118;
    int mWidth = 0;
    int mHeight = 0;
    unsigned char * img = nullptr;

};


#endif // QWIDGET_FRAME_H
