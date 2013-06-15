#include "renderers.h"
#include <QTime>
#include <QDebug>

QImage drawSideBySide(const QImage &imgL, const QImage &imgR, int panX, int panY, int finalwidth, int finalheight, float zoom, bool mirrorL, bool mirrorR){
    QTime starttime = QTime::currentTime();

    if(finalwidth <= 0 || finalheight <= 0){
        finalwidth = imgL.width() + imgR.width();
        finalheight = imgL.height();
    }

    if(zoom <= 0.0f){
        zoom = qMin(float(finalwidth) / float(imgL.width()) * 0.5f, float(finalheight) / float(imgL.height()));
    }

    panX *= 0.5f;
    panX += finalwidth * 0.25f - imgL.width()  * zoom * 0.5f;
    panY += finalheight * 0.5f - imgL.height() * zoom * 0.5f;

    QImage final(finalwidth, finalheight, QImage::Format_RGB32);

    QRgb *line;
    QRgb *lineL;
    QRgb *lineR;

    for(int y = 0; y < finalheight; y++){
        line = (QRgb*)final.scanLine(y);
        int cy = (y - panY) / zoom;
        if(cy >= 0 && cy < imgL.height()){
            lineL = (QRgb*)imgL.constScanLine(cy);
            lineR = (QRgb*)imgR.constScanLine(cy);
            for(int x = 0; x < finalwidth; x++){
                int cxl = x;
                int cxr = x;
                if(mirrorL){
                    cxl = (finalwidth * 0.5f) - cxl;
                }
                if(mirrorR){
                    cxr = (finalwidth * 0.5f) - cxr;
                    cxr += finalwidth / 2;
                }else{
                    cxr -= finalwidth / 2;
                }
                cxl = (cxl - panX) / zoom;
                cxr = (cxr - panX) / zoom;

                if(x > finalwidth / 2 && imgR.valid(cxr,cy)){
                    line[x]=lineR[cxr];
                }else if(x < finalwidth / 2 && imgL.valid(cxl,cy)){
                    line[x]=lineL[cxl];
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

    qDebug() << "Draw time: " << starttime.msecsTo(QTime::currentTime()) << "ms";

    return final;
}

