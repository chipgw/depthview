#ifndef RENDERERS_H
#define RENDERERS_H

#include <QImage>
#include <QPixmap>

QImage drawAnglaph(const QImage &imgL, const QImage &imgR);
QImage drawAnglaphHalf(const QImage &imgL, const QImage &imgR);
QImage drawAnglaphGrey(const QImage &imgL, const QImage &imgR);

void drawInterlaced(const QPixmap &imgL, const QPixmap &imgR, int panX, int panY, QPainter &painter, QBitmap &mask, qreal zoom = 0.0);

void drawSideBySide(const QPixmap &imgL, const QPixmap &imgR, int panX, int panY, QPainter &painter, qreal zoom = 0.0, bool mirrorL = false, bool mirrorR = false);

void drawTopBottom(const QPixmap &imgL, const QPixmap &imgR, int panX, int panY, QPainter &painter, qreal zoom = 0.0, bool mirrorL = false, bool mirrorR = false);

void drawSingle(const QPixmap &img, int panX, int panY, QPainter &painter, qreal zoom = 0.0);


#endif // RENDERERS_H
