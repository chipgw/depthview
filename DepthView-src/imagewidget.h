#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include "common.h"
#include "stereorender.h"
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
    QImage imgL, imgR;
    StereoRender *renderer;
    bool smooth;
    bool swapLR;

    explicit ImageWidget(QWidget *parent = 0);
    void loadStereoImage(QString filename);
    void setZoom(float val);
    float getZoom();
    void zoomIn();
    void zoomOut();
    void addZoom(float amount);

public slots:
    void hideCursor();

protected:
    void resizeEvent(QResizeEvent * e);
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    void recalculatescroolmax();
};

#endif // IMAGEWIDGET_H
