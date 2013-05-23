#include "renderers.h"
#include <QTime>
#include <QDebug>

QImage drawTopBottom(const QImage &imgL, const QImage &imgR, int panX, int panY, int finalwidth, int finalheight, float zoom, bool mirrorL, bool mirrorR){
    QTime starttime = QTime::currentTime();

    if(finalwidth <= 0 || finalheight <= 0){
        finalwidth = imgL.width();
        finalheight = imgL.height() + imgR.height();
    }

    if(zoom <= 0.0f){
        zoom = qMin(float(finalwidth) / float(imgL.width()), float(finalheight) / float(imgL.height()) * 0.5f);
    }

    panY *= 0.5f;
    panX += finalwidth  * 0.5f  - imgL.width()  * zoom * 0.5f;
    panY += finalheight * 0.25f - imgL.height() * zoom * 0.5f;

    QImage final(finalwidth, finalheight, QImage::Format_RGB32);

    QRgb *lineOut;
    QRgb *lineIn;

    for(int y = 0; y < finalheight; y++){
        lineOut = (QRgb*)final.scanLine(y);
        int cy = y;

        if(y > finalheight / 2){
            if(mirrorR){
                cy = (finalheight * 0.5f) - cy;
                cy += finalheight / 2;
            }else{
                cy -= finalheight / 2;
            }
            cy = (cy - panY) / zoom;
            if(cy >= 0 && cy < imgR.height()){
                lineIn = (QRgb*)imgR.constScanLine(cy);

                for(int x = 0; x < finalwidth; x++){
                    int cx = (x - panX) / zoom;

                    if(imgR.valid(cx, cy)){
                        lineOut[x] = lineIn[cx];
                    }else{
                        lineOut[x] = qRgb(0,0,0);
                    }
                }
            }
        }
        else if(y < finalheight / 2){
            if(mirrorL){
                cy = (finalheight * 0.5f) - cy;
            }
            cy = (cy - panY) / zoom;

            if(cy >= 0 && cy < imgL.height()){
                lineIn = (QRgb*)imgL.constScanLine(cy);

                for(int x = 0; x < finalwidth; x++){
                    int cx = (x - panX) / zoom;

                    if(imgL.valid(cx, cy)){
                        lineOut[x] = lineIn[cx];
                    }else{
                        lineOut[x] = qRgb(0,0,0);
                    }
                }
            }
        }else{
            for(int x = 0; x < finalwidth; x++){
                lineOut[x] = qRgb(0,0,0);
            }
        }
    }

    qDebug() << "Draw time: " << starttime.msecsTo(QTime::currentTime()) << "ms";

    return final;
}

