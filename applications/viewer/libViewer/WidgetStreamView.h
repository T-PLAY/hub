#ifndef WIDGETSTREAMVIEW_H
#define WIDGETSTREAMVIEW_H

#include <QObject>
#include <QWidget>

class WidgetStreamView : public QWidget {
    Q_OBJECT
public:
    explicit WidgetStreamView(QWidget* parent = nullptr);
//    ~WidgetStreamView();

public slots:
    void setImage(unsigned char* img_ptr, int width, int height);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    //    QImage image;
    //    uchar * imagePtr = nullptr;
    //    static const int imgSize = 118;
    //    int m_iStreamer;
    int mWidth = 0;
    int mHeight = 0;
    unsigned char* img = nullptr;

    //    std::chrono::time_point mStart;
    //    Ui::FormStreamView *ui;
};

#endif // WIDGETSTREAMVIEW_H
