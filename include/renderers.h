#ifndef RENDERERS_H
#define RENDERERS_H

#include <QImage>

QImage drawAnglaph(const QImage &imgL, const QImage &imgR);
QImage drawAnglaphHalf(const QImage &imgL, const QImage &imgR);
QImage drawAnglaphGrey(const QImage &imgL, const QImage &imgR);

QImage drawCheckerboard(const QImage &imgL, const QImage &imgR, int panX, int panY, QSize finalSize = QSize(), qreal zoom = 0.0, QWidget *parent = NULL);

QImage drawInterlaced(const QImage &imgL, const QImage &imgR, int panX, int panY, QSize finalSize = QSize(), qreal zoom = 0.0, bool horizontal = false, QWidget *parent = NULL);

void drawSideBySide(const QImage &imgL, const QImage &imgR, int panX, int panY, QPainter &painter, qreal zoom = 0.0, bool mirrorL = false, bool mirrorR = false);

void drawTopBottom(const QImage &imgL, const QImage &imgR, int panX, int panY, QPainter &painter, qreal zoom = 0.0, bool mirrorL = false, bool mirrorR = false);

void drawSingle(const QImage &img, int panX, int panY, QPainter &painter, qreal zoom = 0.0);


#endif // RENDERERS_H
