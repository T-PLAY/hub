#ifndef WIDGETSTREAMVIEW_H
#define WIDGETSTREAMVIEW_H

#include <QObject>
#include <QWidget>

class WidgetStreamView : public QWidget {
    Q_OBJECT
public:
    explicit WidgetStreamView(QWidget* parent = nullptr);

public slots:
    void setImage(unsigned char* img_ptr, int width, int height);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    int mWidth = 0;
    int mHeight = 0;
    unsigned char* img = nullptr;
};

#endif // WIDGETSTREAMVIEW_H
