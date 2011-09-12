#include "stereorender.h"
#include "imagewidget.h"
#include <cmath>
#include <QTime>
#include <QDebug>

StereoRender::StereoRender(){
}

QImage StereoRender::draw(QImage imgL, QImage imgR, float panX, float panY, int finalwidth, int finalheight){
    // Default Stereo Draw is Anglaph
    QImage final;

    QTime starttime = QTime::currentTime();

    if(finalwidth > 0 && finalheight > 0){
        final = QImage(finalwidth,finalheight,QImage::Format_ARGB32);
        panX += finalwidth/2 - imgL.width()/2;
        panY += finalheight/2 - imgL.height()/2;
    }
    else{
        final = QImage(imgL.width(),imgL.height(),QImage::Format_ARGB32);
    }
    int minx = qMin(final.width(), qMin(imgL.width(), imgR.width()));
    int miny = qMin(final.height(), qMin(imgL.height(), imgR.height()));
    QRgb *line;
    for(int y=0;y<final.height();y++){
        line = (QRgb *)final.scanLine(y);
        for(int x=0;x<final.width();x++){
            if(x-panX >= minx || y-panY >= miny || x < panX || y < panY){
                line[x] = qRgb(0,0,0);
            }
            else{
                QRgb Lpixel=imgL.pixel(x-panX,y-panY);
                QRgb Rpixel=imgR.pixel(x-panX,y-panY);
                int red   = qRed(Rpixel)*colormult     + qGray(Rpixel)*!colormult;
                int green = qGreen(Lpixel)*colormult   + qGray(Lpixel)*!colormult;
                int blue  = qBlue(Lpixel)*colormult    + qGray(Lpixel)*!colormult;
                line[x] = qRgb(red,green,blue);
            }
        }
    }

    qDebug() << "Draw time: " << starttime.msecsTo(QTime::currentTime()) << "ms";

    return final;
}

float StereoRender::colormult = 1;
