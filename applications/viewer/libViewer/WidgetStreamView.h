#ifndef WIDGETSTREAMVIEW_H
#define WIDGETSTREAMVIEW_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QTableWidget>
#include <QImage>

#include <SensorSpec.hpp>


class WidgetStreamView : public QWidget {
    Q_OBJECT
public:
    explicit WidgetStreamView(QWidget* parent = nullptr);

public slots:
//    virtual void setData(unsigned char* img_ptr, std::vector<int> dims, Stream::Format format) = 0;
    virtual void setData(unsigned char* img_ptr, size_t size, std::vector<int> dims, hub::SensorSpec::Format format) = 0;

protected:
protected:
    hub::SensorSpec::Format mFormat = hub::SensorSpec::Format::NONE;
    unsigned char* mData = nullptr;
};

class WidgetStreamView2D : public WidgetStreamView {
    Q_OBJECT
public:
//    WidgetStreamView2D(int imagePixelWidth, int imagePixelHeight, double imageUnitWidth = 1.0, double imageUnitHeight = 1.0, QWidget* parent = nullptr);
    explicit WidgetStreamView2D(QWidget* parent = nullptr);

//    void setRatio(double newRatio);
//    void setRealSize(double realWidth, double realHeight);
    void init(int imagePixelWidth, int imagePixelHeight, double imageUnitWidth = 0.0, double imageUnitHeight = 0.0);

//    const double & getPixelPerUnit() const;
    void clear();


public slots:
//    void setData(unsigned char* img_ptr, std::vector<int> dims, Stream::Format format) override;
    void setData(unsigned char* img_ptr, size_t size, std::vector<int> dims, hub::SensorSpec::Format format) override;


    void onPixelPerUnitChanged();

private:
    void updateImage();

protected:
    void paintEvent(QPaintEvent* event) override;
//    void resizeEvent(QResizeEvent* event) override;

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

    int mImagePixelWidth = 0;
    int mImagePixelHeight = 0;

    double mImageUnitWidth;
    double mImageUnitHeight;

    double mRatio;
    double mHPixelPerUnit;
    double mVPixelPerUnit;



//    double mUnitPerImageHPixel = 1.0;
//    double mUnitPerImageVPixel = 1.0;

//    const double * mCanvasPixelPerUnit = nullptr;
    double mCanvasPixelPerUnit = 1.0;
    int mCanvasPixelWidth;
    int mCanvasPixelHeight;


//    double mHPixelPerUnit;
//    double mVPixelPerUnit;

//    double mPixelWidthPerMillimeters;
//    double mPixelHeightPerMillimeters;
//    QPixmap * m_grid = nullptr;
//    QImage * m_grid = nullptr;
    double mRotateDeg = 0.0;
    bool mShowGrid = false;

public:
    void setRotateDeg(double newRotateDeg);
//    void setCanvasPixelPerUnit(const double &newCanvasPixelPerUnit);

    const int & getCanvasPixelWidth() const;
    const int & getCanvasPixelHeight() const;
    double & getCanvasPixelPerUnit();
    void setShowGrid(bool newShowGrid);
    const double & getImageUnitWidth() const;
    const double & getImageUnitHeight() const;
};

class WidgetStreamView1D : public WidgetStreamView {
    Q_OBJECT
public:
    explicit WidgetStreamView1D(QWidget* parent = nullptr);
    ~WidgetStreamView1D();

public slots:
//    void setData(unsigned char* img_ptr, std::vector<int> dims, Stream::Format format) override;
    void setData(unsigned char* img_ptr, size_t size, std::vector<int> dims, hub::SensorSpec::Format format) override;

protected:
private:
    QLabel* mLabel = nullptr;
};

#endif // WIDGETSTREAMVIEW_H
