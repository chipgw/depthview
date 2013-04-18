#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include "common.h"
#include "anglaphrender.h"
#include "sidebysiderender.h"
#include "interlacedrender.h"
#include "checkerboardrender.h"
#include "singlerender.h"
#include <QScrollBar>
#include <QTimer>

class ImageWidget : public QWidget {
    Q_OBJECT
    QScrollBar *hBar;
    QScrollBar *vBar;
    QPoint lastmousepos;
    float zoom;

    QTimer* mouseTimer;
public:
    enum DrawMode{
        AnglaphFull,
        AnglaphHalf,
        AnglaphGreyscale,
        SidebySide,
        SidebySideMLeft,
        SidebySideMRight,
        SidebySideMBoth,
        InterlacedHorizontal,
        InterlacedVertical,
        Checkerboard,
        MonoLeft,
        MonoRight
    };

    QImage imgL, imgR;
    bool swapLR;
    DrawMode mode;

    explicit ImageWidget(QWidget *parent = 0);
    void loadStereoImage(QString filename);
    void setZoom(float val);
    float getZoom();
    void zoomIn();
    void zoomOut();
    void addZoom(float amount);

    QImage draw(const QImage &L, const QImage &R);

public slots:
    void hideCursor();

signals:
    void doubleClicked();

protected:
    void resizeEvent(QResizeEvent * e);
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    void recalculatescroolmax();
};

#endif // IMAGEWIDGET_H