#include "sidebysiderender.h"

SideBySideRender::SideBySideRender(){
}

QImage SideBySideRender::draw(QImage imgL, QImage imgR, int panX, int panY, int finalwidth, int finalheight, float zoom){
    // Default Stereo Draw is Anglaph
    QTime starttime = QTime::currentTime();

    if(finalwidth > 0 && finalheight > 0){
    }
    else{
        finalwidth = imgL.width()*2;
        finalheight = imgL.height();
    }

    if(zoom == 0){
        imgL = imgL.scaledToWidth(finalwidth/2);
        imgR = imgR.scaledToWidth(finalwidth/2);
    }
    else{
        imgL = imgL.scaledToWidth(imgL.width()*zoom/2);
        imgR = imgR.scaledToWidth(imgR.width()*zoom/2);
    }
    panY += finalheight - imgL.height();
    panY /= 2;

    float panX_L = finalwidth/4 - imgL.width()/2;
    float panX_R = finalwidth/4 - imgL.width()/2;

    if(SideBySideRender::mirrorL){
        imgL = imgL.mirrored(true,false);
        panX_L -= panX/2;
    }
    else{
        panX_L += panX/2;
    }
    if(SideBySideRender::mirrorR){
        imgR = imgR.mirrored(true,false);
        panX_R -= panX/2;
    }
    else{
        panX_R += panX/2;
    }


    QImage final(finalwidth,finalheight,QImage::Format_ARGB32);

    QRgb *line;
    for(int y=0;y<finalheight;y++){
        line = (QRgb *)final.scanLine(y);
        for(int x=0;x<finalwidth;x++){
            if(x>finalwidth/2 && imgR.valid(x-finalwidth/2-panX_R,y-panY)){
                line[x]=imgR.pixel(x-finalwidth/2-panX_R,y-panY);
            }
            else if(x<finalwidth/2 && imgL.valid(x-panX_L,y-panY)){
                line[x]=imgL.pixel(x-panX_L,y-panY);
            }
            else{
                line[x] = qRgb(0,0,0);
            }
        }
    }

    qDebug() << "Draw time: " << starttime.msecsTo(QTime::currentTime()) << "ms";

    return final;
}

bool SideBySideRender::mirrorL = false;
bool SideBySideRender::mirrorR = false;
