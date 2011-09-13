#include "sidebysiderender.h"

SideBySideRender::SideBySideRender(){
}

QImage SideBySideRender::draw(QImage imgL, QImage imgR, float panX, float panY, int finalwidth, int finalheight, float zoom){
    // Default Stereo Draw is Anglaph
    QTime starttime = QTime::currentTime();

    if(finalwidth > 0 && finalheight > 0){
    }
    else{
        finalwidth = imgL.width()*2;
        finalheight = imgL.height();
    }

    if(zoom == 0){
        imgL = imgL.scaledToWidth(finalwidth);
        imgR = imgR.scaledToWidth(finalwidth);
    }
    else{
        imgL = imgL.scaledToWidth(imgL.width()*zoom);
        imgR = imgR.scaledToWidth(imgR.width()*zoom);
    }

    panX += finalwidth/2 - imgL.width()/2;
    panY += finalheight/2 - imgL.height()/4;

    QImage final(finalwidth,finalheight,QImage::Format_ARGB32);

    int minx = qMin(finalwidth, qMin(imgL.width(), imgR.width()));
    int miny = qMin(finalheight, qMin(imgL.height()/2, imgR.height()/2));
    QRgb *line;
    for(int y=0;y<finalheight;y++){
        line = (QRgb *)final.scanLine(y);
        for(int x=0;x<finalwidth;x++){
            if(x-panX >= minx || y-panY >= miny || x < panX || y < panY){
                line[x] = qRgb(0,0,0);
            }
            else{
                if(imgR.valid((x-finalwidth/2-panX)*2,(y-panY)*2)){
                    line[x]=imgR.pixel((x-finalwidth/2-panX)*2,(y-panY)*2);
                }
                else if(imgL.valid((x-panX)*2,(y-panY)*2)){
                    line[x]=imgL.pixel((x-panX)*2,(y-panY)*2);
                }
            }
        }
    }

    qDebug() << "Draw time: " << starttime.msecsTo(QTime::currentTime()) << "ms";

    return final;
}
