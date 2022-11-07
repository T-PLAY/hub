#ifndef QSCROLLAREAGRID_H
#define QSCROLLAREAGRID_H

#include <QScrollArea>
#include <QWheelEvent>
#include <QWidget>

class QScrollAreaGrid : public QScrollArea
{
    Q_OBJECT
  public:
    QScrollAreaGrid( QWidget* parent = nullptr );

    void setScrollAreaTop( QScrollArea* newScrollAreaTop );

    void setScrollAreaLeft( QScrollArea* newScrollAreaLeft );

    void setCanvasPixelPerUnit( double* newCanvasPixelPerUnit );

  signals:
    void pixelPerUnitChanged();

  protected:
    void wheelEvent( QWheelEvent* event ) override;
    void mousePressEvent( QMouseEvent* event ) override;
    void mouseReleaseEvent( QMouseEvent* event ) override;
    void mouseMoveEvent( QMouseEvent* event ) override;

  private:
    double* mCanvasPixelPerUnit = nullptr;

    int mousePosX;
    int mousePosY;
    int hSliderPos;
    int vSliderPos;
    bool leftMouseDown = false;

    QScrollArea* m_scrollAreaTop  = nullptr;
    QScrollArea* m_scrollAreaLeft = nullptr;
};

#endif // QSCROLLAREAGRID_H
