#include "stereorender.h"
#include "imagewidget.h"

StereoRender::StereoRender(){
}

QImage StereoRender::draw(QImage imgL, QImage imgR, int panX, int panY, int finalwidth, int finalheight, float zoom){
    // Default Stereo Draw is Anglaph
    QTime starttime = QTime::currentTime();

    if(finalwidth <= 0 || finalheight <= 0){
        finalwidth = imgL.width();
        finalheight = imgL.height();
    }

    if(zoom == 0.0f){
        imgL = imgL.scaled(finalwidth,finalheight,Qt::KeepAspectRatio);
        imgR = imgR.scaled(finalwidth,finalheight,Qt::KeepAspectRatio);
    }
    else{
        imgL = imgL.scaled(imgL.width()*zoom,imgL.height()*zoom,Qt::KeepAspectRatio);
        imgR = imgR.scaled(imgR.width()*zoom,imgR.height()*zoom,Qt::KeepAspectRatio);
    }

    panX += finalwidth/2 - imgL.width()/2;
    panY += finalheight/2 - imgL.height()/2;

    QImage final(finalwidth,finalheight,QImage::Format_ARGB32);

    QRgb *line;
    QRgb *lineL;
    QRgb *lineR;

    for(int y=0;y<final.height();y++){
        line = (QRgb *)final.scanLine(y);
        if(y-panY >= 0 && y-panY < imgL.height()){
            lineL = (QRgb *)imgL.constScanLine(y-panY);
            lineR = (QRgb *)imgR.constScanLine(y-panY);
            for(int x=0;x<final.width();x++){
                if(imgL.valid(x-panX,y-panY)&&imgR.valid(x-panX,y-panY)){
                    line[x] = qRgb(qRed(lineR[x-panX])*colormult     + qGray(lineR[x-panX])*(1-colormult),
                                   qGreen(lineL[x-panX])*colormult   + qGray(lineL[x-panX])*(1-colormult),
                                   qBlue(lineL[x-panX])*colormult    + qGray(lineL[x-panX])*(1-colormult));
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

float StereoRender::colormult = 1;
