#ifndef ANGLAPHRENDER_H
#define ANGLAPHRENDER_H

#include <QImage>

QImage drawAnglaph(const QImage &imgL, const QImage &imgR, int panX, int panY, int finalwidth = 0, int finalheight = 0, float zoom = 0.0f, float colormult = 0.0f, QRgb colorL = qRgb(255, 0, 0), QRgb colorR = qRgb(0, 255, 255));


#endif // ANGLAPHRENDER_H
