#include "anglaphrender.h"
#include <QTime>

QImage drawAnglaph(const QImage &imgL, const QImage &imgR, int panX, int panY, int finalwidth, int finalheight, float zoom, float colormult){
    QTime starttime = QTime::currentTime();

    if(finalwidth <= 0 || finalheight <= 0){
        finalwidth = imgL.width();
        finalheight = imgL.height();
    }

    if(zoom <= 0.0f){
        zoom = qMin((float)finalwidth / (float)imgL.width(), (float)finalheight / (float)imgL.height());
    }

    panX += (finalwidth*0.5f  - imgL.width()  * zoom * 0.5f);
    panY += (finalheight*0.5f - imgL.height() * zoom * 0.5f);

    QImage final(finalwidth,finalheight,QImage::Format_RGB32);

    QRgb *line;
    QRgb *lineL;
    QRgb *lineR;

    for(int y=0;y<final.height();y++){
        line = (QRgb *)final.scanLine(y);
        int cy = (y-panY)/zoom;
        if(cy >= 0 && cy < imgL.height()){
            lineL = (QRgb *)imgL.constScanLine(cy);
            lineR = (QRgb *)imgR.constScanLine(cy);
            for(int x=0;x<final.width();x++){
                int cx = (x-panX)/zoom;
                if(imgL.valid(cx,cy) && imgR.valid(cx,cy)){
                    line[x] = qRgb(qRed(  lineR[cx])*colormult + qGray(lineR[cx])*(1-colormult),
                                   qGreen(lineL[cx])*colormult + qGray(lineL[cx])*(1-colormult),
                                   qBlue( lineL[cx])*colormult + qGray(lineL[cx])*(1-colormult));
                }
                else{
                    line[x] = qRgb(0,0,0);
                }
            }
        }
        else{
            for(int x=0;x<final.width();x++)
                line[x] = qRgb(0,0,0);
        }
    }

    qDebug() << "Draw time: " << starttime.msecsTo(QTime::currentTime()) << "ms";

    return final;
}

