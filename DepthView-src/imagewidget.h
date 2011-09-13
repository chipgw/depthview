#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QScrollBar>
#include "stereorender.h"
#include "sidebysiderender.h"

class ImageWidget : public QWidget {
    Q_OBJECT
    QScrollBar *hBar;
    QScrollBar *vBar;
    QPoint lastmousepos;
public:
    QImage imgL, imgR;
    float zoom;
    StereoRender *renderer;
    bool smooth;

    explicit ImageWidget(QWidget *parent = 0);
    void loadStereoImage(QString filename);

signals:

public slots:

protected:
    void resizeEvent(QResizeEvent * e);
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
};

#endif // IMAGEWIDGET_H
