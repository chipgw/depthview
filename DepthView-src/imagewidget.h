#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include "common.h"
#include "stereorender.h"
#include "sidebysiderender.h"
#include "interlacedrender.h"
#include "checkerboardrender.h"
#include "singlerender.h"
#include <QScrollBar>

class ImageWidget : public QWidget {
    Q_OBJECT
    QScrollBar *hBar;
    QScrollBar *vBar;
    QPoint lastmousepos;
    float zoom;
public:
    QImage imgL, imgR;
    StereoRender *renderer;
    bool smooth;

    explicit ImageWidget(QWidget *parent = 0);
    void loadStereoImage(QString filename);
    void setZoom(float val);
    float getZoom();
    void zoomIn();
    void zoomOut();

signals:

public slots:

protected:
    void resizeEvent(QResizeEvent * e);
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    void recalculatescroolmax();
};

#endif // IMAGEWIDGET_H
