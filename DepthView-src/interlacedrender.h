#ifndef INTERLACEDRENDER_H
#define INTERLACEDRENDER_H

#include "common.h"
#include "stereorender.h"
#include "QWidget"

class InterlacedRender : public StereoRender
{
public:
    static bool lFirst;
    static bool horizontal;
    QWidget *parent;

    InterlacedRender();
    InterlacedRender(QWidget *Parent);

    virtual QImage draw(QImage imgL, QImage imgR, int panX, int panY, int finalwidth = 0, int finalheight = 0, float zoom = 0);
};

#endif // INTERLACEDRENDER_H
