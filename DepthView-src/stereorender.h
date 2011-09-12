#ifndef STEREORENDER_H
#define STEREORENDER_H

#include <QPainter>

class StereoRender
{
public:
    static float colormult;

    StereoRender();

    virtual QImage draw(QImage imgL, QImage imgR, float panX, float panY, int finalwidth = 0, int finalheight = 0);
};

#endif // STEREORENDER_H
