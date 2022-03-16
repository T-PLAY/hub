#ifndef WIDGETSTREAMVIEW_H
#define WIDGETSTREAMVIEW_H

#include <QObject>
#include <QWidget>

#include <QLabel>
#include <QTableWidget>
#include <stream.h>

class WidgetStreamView : public QWidget {
    Q_OBJECT
public:
    explicit WidgetStreamView(QWidget* parent = nullptr);

public slots:
    virtual void setData(unsigned char* img_ptr, std::vector<int> dims, Stream::Format format) = 0;

protected:
protected:
    Stream::Format mFormat = Stream::Format::NONE;
    unsigned char* mData = nullptr;
};

class WidgetStreamView2D : public WidgetStreamView {
    Q_OBJECT
public:
    explicit WidgetStreamView2D(QWidget* parent = nullptr);

public slots:
    void setData(unsigned char* img_ptr, std::vector<int> dims, Stream::Format format) override;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    int mWidth;
    int mHeight;
};

class WidgetStreamView1D : public WidgetStreamView {
    Q_OBJECT
public:
    explicit WidgetStreamView1D(QWidget* parent = nullptr);
    ~WidgetStreamView1D();

public slots:
    void setData(unsigned char* img_ptr, std::vector<int> dims, Stream::Format format) override;

protected:
private:
    QLabel* mLabel = nullptr;
};

#endif // WIDGETSTREAMVIEW_H
