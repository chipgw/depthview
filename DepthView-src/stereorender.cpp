#include "stereorender.h"
#include "imagewidget.h"

StereoRender::StereoRender(){
}

QImage StereoRender::draw(QImage imgL, QImage imgR, int panX, int panY, int finalwidth, int finalheight, float zoom){
    // Default Stereo Draw is Anglaph
    QTime starttime = QTime::currentTime();

    if(finalwidth > 0 && finalheight > 0){
    }
    else{
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
    for(int y=0;y<final.height();y++){
        line = (QRgb *)final.scanLine(y);
        for(int x=0;x<final.width();x++){
            if(imgL.valid(x-panX,y-panY)&&imgR.valid(x-panX,y-panY)){
                QRgb Lpixel=imgL.pixel(x-panX,y-panY);
                QRgb Rpixel=imgR.pixel(x-panX,y-panY);
                int red   = qRed(Rpixel)*colormult     + qGray(Rpixel)*(1-colormult);
                int green = qGreen(Lpixel)*colormult   + qGray(Lpixel)*(1-colormult);
                int blue  = qBlue(Lpixel)*colormult    + qGray(Lpixel)*(1-colormult);
                line[x] = qRgb(red,green,blue);
            }
            else{
                line[x] = qRgb(0,0,0);
            }
        }
    }

    qDebug() << "Draw time: " << starttime.msecsTo(QTime::currentTime()) << "ms";

    return final;
}

float StereoRender::colormult = 1;
