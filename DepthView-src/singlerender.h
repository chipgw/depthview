#ifndef SINGLERENDER_H
#define SINGLERENDER_H

#include "common.h"
#include "stereorender.h"

class SingleRender : public StereoRender {
public:
    static bool left;
    SingleRender();

    QImage draw(QImage imgL, QImage imgR, int panX, int panY, int finalwidth = 0, int finalheight = 0, float zoom = 0);
};

#endif // SINGLERENDER_H
