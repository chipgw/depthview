#ifndef INTERLACEDRENDER_H
#define INTERLACEDRENDER_H

#include "common.h"

QImage drawInterlaced(const QImage &imgL, const QImage &imgR, int panX, int panY, int finalwidth = 0, int finalheight = 0, float zoom = 0.0f, bool horizontal = false, QWidget *parent = NULL);

#endif // INTERLACEDRENDER_H
