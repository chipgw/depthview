#ifndef SIDEBYSIDERENDER_H
#define SIDEBYSIDERENDER_H

#include "common.h"

QImage drawSideBySide(const QImage &imgL, const QImage &imgR, int panX, int panY, int finalwidth = 0, int finalheight = 0, float zoom = 0, bool mirrorL = false, bool mirrorR = false);

#endif // SIDEBYSIDERENDER_H
