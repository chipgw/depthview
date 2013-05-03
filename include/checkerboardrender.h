#ifndef CHECKERBOARDRENDER_H
#define CHECKERBOARDRENDER_H

#include <QImage>

QImage drawCheckerboard(const QImage &imgL, const QImage &imgR, int panX, int panY, int finalwidth = 0, int finalheight = 0, float zoom = 0.0f, QWidget *parent = NULL);

#endif // CHECKERBOARDRENDER_H
