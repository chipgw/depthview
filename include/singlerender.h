#ifndef SINGLERENDER_H
#define SINGLERENDER_H

#include "common.h"

QImage drawSingle(const QImage &img, int panX, int panY, int finalwidth = 0, int finalheight = 0, float zoom = 0.0f);

#endif // SINGLERENDER_H
