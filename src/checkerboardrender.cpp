#include "renderers.h"
#include <QWidget>

QImage drawCheckerboard(const QImage &imgL, const QImage &imgR, int panX, int panY, int finalwidth, int finalheight, float zoom, QWidget *parent){
    if(zoom <= 0.0f){
        zoom = qMin(float(finalwidth) / float(imgL.width()), float(finalheight) / float(imgL.height()));
    }
    QPoint pos = parent->mapToGlobal(QPoint());

    panX += (finalwidth  * 0.5f - imgL.width()  * zoom * 0.5f);
    panY += (finalheight * 0.5f - imgL.height() * zoom * 0.5f);

    QImage final(finalwidth, finalheight, QImage::Format_RGB32);

    QRgb *line;
    QRgb *lineL;
    QRgb *lineR;

    for(int y = 0; y < final.height(); y++){
        line = (QRgb*)final.scanLine(y);
        int cy = (y - panY) / zoom;
        if(cy >= 0 && cy < imgL.height()){
            lineL = (QRgb*)imgL.constScanLine(cy);
            lineR = (QRgb*)imgR.constScanLine(cy);
            for(int x = 0; x < final.width(); x++){
                int cx = (x - panX) / zoom;
                if(imgL.valid(cx,cy) && imgR.valid(cx,cy)){
                    if((pos.x() + x) % 2 == (pos.y() + y) % 2){
                        line[x] = lineR[cx];
                    }else{
                        line[x] = lineL[cx];
                    }
                }else{
                    line[x] = qRgb(0,0,0);
                }
            }
        }else{
            for(int x = 0; x < final.width(); x++){
                line[x] = qRgb(0,0,0);
            }
        }
    }

    return final;
}

