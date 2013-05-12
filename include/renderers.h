#ifndef RENDERERS_H
#define RENDERERS_H

#include <QImage>

QImage drawAnglaph(const QImage &imgL, const QImage &imgR, int panX, int panY, int finalwidth = 0, int finalheight = 0, float zoom = 0.0f, float colormult = 0.0f, QRgb colorL = qRgb(255, 0, 0), QRgb colorR = qRgb(0, 255, 255));

QImage drawCheckerboard(const QImage &imgL, const QImage &imgR, int panX, int panY, int finalwidth = 0, int finalheight = 0, float zoom = 0.0f, QWidget *parent = NULL);

QImage drawInterlaced(const QImage &imgL, const QImage &imgR, int panX, int panY, int finalwidth = 0, int finalheight = 0, float zoom = 0.0f, bool horizontal = false, QWidget *parent = NULL);

QImage drawSideBySide(const QImage &imgL, const QImage &imgR, int panX, int panY, int finalwidth = 0, int finalheight = 0, float zoom = 0.0f, bool mirrorL = false, bool mirrorR = false);

QImage drawSingle(const QImage &img, int panX, int panY, int finalwidth = 0, int finalheight = 0, float zoom = 0.0f);


#endif // RENDERERS_H
