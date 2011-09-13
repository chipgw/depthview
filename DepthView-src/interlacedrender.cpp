#include "interlacedrender.h"

InterlacedRender::InterlacedRender(){
}
InterlacedRender::InterlacedRender(QWidget * Parent){
    parent = Parent;
}

QImage InterlacedRender::draw(QImage imgL, QImage imgR, int panX, int panY, int finalwidth, int finalheight, float zoom){
    // Default Stereo Draw is Anglaph
    QTime starttime = QTime::currentTime();

    bool first = lFirst;
    if(parent){
        if(horizontal){
            first = parent->pos().x()%2;
        }
        else{
            first = parent->pos().y()%2;
        }
        if(first != lFirst){
            first = ! first;
        }
    }
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
    if(this->horizontal){
        for(int y=0;y<final.height();y++){
            line = (QRgb *)final.scanLine(y);
            for(int x=0;x<final.width();x++){
                if(imgL.valid(x-panX,y-panY)&&imgR.valid(x-panX,y-panY)){
                    if(y%2 == first){
                        line[x]=imgR.pixel(x-panX,y-panY);
                    }else{
                        line[x]=imgL.pixel(x-panX,y-panY);
                    }
                }
                else{
                    line[x] = qRgb(0,0,0);
                }
            }
        }
    }
    else{
        for(int y=0;y<final.height();y++){
            line = (QRgb *)final.scanLine(y);
            for(int x=0;x<final.width();x++){
                if(imgL.valid(x-panX,y-panY)&&imgR.valid(x-panX,y-panY)){
                    if(x%2 == first){
                        line[x]=imgR.pixel(x-panX,y-panY);
                    }else{
                        line[x]=imgL.pixel(x-panX,y-panY);
                    }
                }
                else{
                    line[x] = qRgb(0,0,0);
                }
            }
        }
    }

    qDebug() << "Draw time: " << starttime.msecsTo(QTime::currentTime()) << "ms";

    return final;
}

bool InterlacedRender::horizontal=true;
bool InterlacedRender::lFirst=true;
