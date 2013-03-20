#ifndef CHECKERBOARDRENDER_H
#define CHECKERBOARDRENDER_H

#include "common.h"
#include "stereorender.h"

class CheckerBoardRender : public StereoRender
{
public:
    static bool lFirst;
    QWidget *parent;

    CheckerBoardRender();
    CheckerBoardRender(QWidget *Parent);

    QImage draw(const QImage &imgL, const QImage &imgR, int panX, int panY, int finalwidth = 0, int finalheight = 0, float zoom = 0);
};

#endif // CHECKERBOARDRENDER_H
