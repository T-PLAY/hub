#pragma once

#include <QImage>
#include <QLabel>
#include <QObject>
#include <QTableWidget>
#include <QWidget>

#include <SensorSpec.hpp>

static const std::vector<double> g_stepSizes { 1.0, 2.0, 5.0, 10.0, 20.0, 25.0, 50.0, 75.0, 100.0 };

class WidgetStreamView : public QWidget
{
    Q_OBJECT
  public:
    explicit WidgetStreamView( QWidget* parent = nullptr );

  public slots:
    virtual void setData( unsigned char* img_ptr,
                          size_t size,
                          std::vector<int> dims,
                          hub::SensorSpec::Format format ) = 0;

  protected:
  protected:
    hub::SensorSpec::Format mFormat = hub::SensorSpec::Format::NONE;
    unsigned char* mData            = nullptr;
};

class WidgetStreamView2D : public WidgetStreamView
{
    Q_OBJECT
  public:
    explicit WidgetStreamView2D( QWidget* parent = nullptr );

    void init( int imagePixelWidth,
               int imagePixelHeight,
               double imageUnitWidth  = 0.0,
               double imageUnitHeight = 0.0 );

  signals:
    void inited();

  public:
    void clear();

  public slots:
    void setData( unsigned char* img_ptr,
                  size_t size,
                  std::vector<int> dims,
                  hub::SensorSpec::Format format ) override;

    void onPixelPerUnitChanged();

  private:
    void updateImage();

  protected:
    void paintEvent( QPaintEvent* event ) override;

  private:
    QImage* m_image = nullptr;

    int mImagePixelWidth  = 0;
    int mImagePixelHeight = 0;

    double mImageUnitWidth  = 0;
    double mImageUnitHeight = 0;

    double mRatio         = 1.0;
    double mHPixelPerUnit = 1.0;
    double mVPixelPerUnit = 1.0;

    double mCanvasPixelPerUnit = 1.0;
    int mCanvasPixelWidth      = 0.0;
    int mCanvasPixelHeight     = 0.0;

    double mRotateDeg = 0.0;
    bool mShowGrid    = false;

  public:
    void setRotateDeg( double newRotateDeg );

    const int& getCanvasPixelWidth() const;
    const int& getCanvasPixelHeight() const;
    double& getCanvasPixelPerUnit();
    void setShowGrid( bool newShowGrid );
    const double& getImageUnitWidth() const;
    const double& getImageUnitHeight() const;
};

class WidgetStreamView1D : public WidgetStreamView
{
    Q_OBJECT
  public:
    explicit WidgetStreamView1D( QWidget* parent = nullptr );
    ~WidgetStreamView1D();

  public slots:
    void setData( unsigned char* img_ptr,
                  size_t size,
                  std::vector<int> dims,
                  hub::SensorSpec::Format format ) override;

  protected:
  private:
    QLabel* mLabel = nullptr;
};
