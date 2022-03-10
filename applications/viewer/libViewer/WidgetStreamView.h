#ifndef WIDGETSTREAMVIEW_H
#define WIDGETSTREAMVIEW_H

#include <QObject>
#include <QWidget>

#include <stream.h>
#include <QLabel>
#include <QTableWidget>

class WidgetStreamView : public QWidget {
    Q_OBJECT
public:
    explicit WidgetStreamView(QWidget* parent = nullptr);

public slots:
    virtual void setData(unsigned char* img_ptr, std::vector<int> dims, Stream::Format format) = 0;

protected:
    //    void paintEvent(QPaintEvent* event) override;

protected:
    Stream::Format mFormat = Stream::Format::NONE;
    unsigned char* mData = nullptr;
//    std::vector<int> mDims;
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
//    void paintEvent(QPaintEvent* event) override;

private:
//    QTableWidget * mTable = nullptr;
    QLabel * mLabel = nullptr;
//    int mWidth;
//    int mHeight;
};

#endif // WIDGETSTREAMVIEW_H
