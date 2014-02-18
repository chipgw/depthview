#ifndef RENDERERS_H
#define RENDERERS_H

#include <QImage>

QImage drawAnglaph(const QImage &imgL, const QImage &imgR, int panX, int panY, QSize finalSize = QSize(), float zoom = 0.0f);
QImage drawAnglaphHalf(const QImage &imgL, const QImage &imgR, int panX, int panY, QSize finalSize = QSize(), float zoom = 0.0f);
QImage drawAnglaphGrey(const QImage &imgL, const QImage &imgR, int panX, int panY, QSize finalSize = QSize(), float zoom = 0.0f);

QImage drawCheckerboard(const QImage &imgL, const QImage &imgR, int panX, int panY, QSize finalSize = QSize(), float zoom = 0.0f, QWidget *parent = NULL);

QImage drawInterlaced(const QImage &imgL, const QImage &imgR, int panX, int panY, QSize finalSize = QSize(), float zoom = 0.0f, bool horizontal = false, QWidget *parent = NULL);

void drawSideBySide(const QImage &imgL, const QImage &imgR, int panX, int panY, QPainter &painter, float zoom = 0.0f, bool mirrorL = false, bool mirrorR = false);

void drawTopBottom(const QImage &imgL, const QImage &imgR, int panX, int panY, QPainter &painter, float zoom = 0.0f, bool mirrorL = false, bool mirrorR = false);

void drawSingle(const QImage &img, int panX, int panY, QPainter &painter, float zoom = 0.0f);


#endif // RENDERERS_H
