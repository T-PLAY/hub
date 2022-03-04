#ifndef WIDGETSTREAMVIEW_H
#define WIDGETSTREAMVIEW_H

#include <QObject>
#include <QWidget>

#include <stream.h>

class WidgetStreamView : public QWidget {
    Q_OBJECT
public:
    explicit WidgetStreamView(QWidget* parent = nullptr);

public slots:
    void setImage(unsigned char* img_ptr, int width, int height, Stream::Format format);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    int mWidth = 0;
    int mHeight = 0;
    Stream::Format mFormat = Stream::Format::NONE;

    unsigned char* img = nullptr;
};

#endif // WIDGETSTREAMVIEW_H
