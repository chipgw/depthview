#ifndef CHECKERBOARDRENDER_H
#define CHECKERBOARDRENDER_H

#include "common.h"

QImage drawCheckerboard(const QImage &imgL, const QImage &imgR, int panX, int panY, int finalwidth = 0, int finalheight = 0, float zoom = 0, QWidget *parent = NULL);

#endif // CHECKERBOARDRENDER_H
