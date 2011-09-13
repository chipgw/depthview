#ifndef SIDEBYSIDERENDER_H
#define SIDEBYSIDERENDER_H

#include "stereorender.h"

class SideBySideRender : public StereoRender
{
public:
    static bool mirrorL;
    static bool mirrorR;

    SideBySideRender();

    QImage draw(QImage imgL, QImage imgR, float panX, float panY, int finalwidth = 0, int finalheight = 0, float zoom = 0);
};

#endif // SIDEBYSIDERENDER_H
