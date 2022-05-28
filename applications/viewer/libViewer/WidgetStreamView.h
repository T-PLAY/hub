#ifndef WIDGETSTREAMVIEW_H
#define WIDGETSTREAMVIEW_H

#include <QObject>
#include <QWidget>

#include <QLabel>
#include <QTableWidget>
#include <stream.h>

#include <QImage>

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
    WidgetStreamView2D(int imagePixelWidth, int imagePixelHeight, double imageUnitWidth = 1.0, double imageUnitHeight = 1.0, QWidget* parent = nullptr);
    explicit WidgetStreamView2D(QWidget* parent = nullptr);

//    void setRatio(double newRatio);
//    void setRealSize(double realWidth, double realHeight);
    void init(int imagePixelWidth, int imagePixelHeight, double imageUnitWidth = 1.0, double imageUnitHeight = 1.0);

//    const double & getPixelPerUnit() const;

    void setRotateDeg(double newRotateDeg);

    void setCanvasPixelPerUnit(const double &newCanvasPixelPerUnit);

    const int & getCanvasPixelWidth() const;
    const int & getCanvasPixelHeight() const;

public slots:
    void setData(unsigned char* img_ptr, std::vector<int> dims, Stream::Format format) override;
    void setData(unsigned char* img_ptr, size_t size, std::vector<int> dims, Stream::Format format);

    void onPixelPerUnitChanged();

private:
    void updateImage();

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    QImage * m_image = nullptr;
//    QPixmap * m_image = nullptr;

//    const int mImagePixelWidth;
//    const int mImagePixelHeight;

//    const double mImageUnitWidth;
//    const double mImageUnitHeight;

//    const double mRatio;
//    const double mHPixelPerUnit;
//    const double mVPixelPerUnit;

    int mImagePixelWidth;
    int mImagePixelHeight;

    double mImageUnitWidth;
    double mImageUnitHeight;

    double mRatio;
    double mHPixelPerUnit;
    double mVPixelPerUnit;



//    double mUnitPerImageHPixel = 1.0;
//    double mUnitPerImageVPixel = 1.0;

    const double * mCanvasPixelPerUnit = nullptr;
    int mCanvasPixelWidth;
    int mCanvasPixelHeight;


//    double mHPixelPerUnit;
//    double mVPixelPerUnit;

//    double mPixelWidthPerMillimeters;
//    double mPixelHeightPerMillimeters;
//    QPixmap * m_grid = nullptr;
//    QImage * m_grid = nullptr;
    double mRotateDeg = 0.0;

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
