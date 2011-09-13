#ifndef STEREORENDER_H
#define STEREORENDER_H

#include <QImage>
#include <QTime>
#include <QDebug>

class StereoRender
{
public:
    static float colormult;

    StereoRender();

    virtual QImage draw(QImage imgL, QImage imgR, int panX, int panY, int finalwidth = 0, int finalheight = 0, float zoom = 0);
};

#endif // STEREORENDER_H
