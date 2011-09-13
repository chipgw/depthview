#include "stereorender.h"
#include "imagewidget.h"

StereoRender::StereoRender(){
}

QImage StereoRender::draw(QImage imgL, QImage imgR, float panX, float panY, int finalwidth, int finalheight, float zoom){
    // Default Stereo Draw is Anglaph
    QTime starttime = QTime::currentTime();

    if(finalwidth > 0 && finalheight > 0){
    }
    else{
        finalwidth = imgL.width();
        finalheight = imgL.height();
    }

    if(zoom == 0){
        imgL = imgL.scaled(finalwidth,finalheight,Qt::KeepAspectRatio);
        imgR = imgR.scaled(finalwidth,finalheight,Qt::KeepAspectRatio);
    }
    else{
        imgL = imgL.scaled(imgL.width()*zoom,imgL.height()*zoom,Qt::KeepAspectRatio);
        imgR = imgR.scaled(imgR.width()*zoom,imgR.width()*zoom,Qt::KeepAspectRatio);
    }

    panX += finalwidth/2 - imgL.width()/2;
    panY += finalheight/2 - imgL.height()/2;

    QImage final(finalwidth,finalheight,QImage::Format_ARGB32);

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
