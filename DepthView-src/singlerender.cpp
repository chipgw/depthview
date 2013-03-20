#include "singlerender.h"

SingleRender::SingleRender(){
}

QImage SingleRender::draw(const QImage &imgL, const QImage &imgR, int panX, int panY, int finalwidth, int finalheight, float zoom){
    QTime starttime = QTime::currentTime();

    if(finalwidth <= 0 || finalheight <= 0){
        finalwidth = imgL.width();
        finalheight = imgL.height();
    }

    QImage in;
    if(left){
        if(zoom == 0.0f){
            in = imgL.scaled(finalwidth,finalheight,Qt::KeepAspectRatio);
        }
        else{
            in = imgL.scaled(imgL.width()*zoom,imgL.height()*zoom,Qt::KeepAspectRatio);
        }
    }
    else{
        if(zoom == 0.0f){
            in = imgR.scaled(finalwidth,finalheight,Qt::KeepAspectRatio);
        }
        else{
            in = imgR.scaled(imgR.width()*zoom,imgR.height()*zoom,Qt::KeepAspectRatio);
        }
    }

    panX += finalwidth  * 0.5f - in.width()  * 0.5f;
    panY += finalheight * 0.5f - in.height() * 0.5f;

    QImage final = in.copy(-panX, -panY, finalwidth, finalheight);


    qDebug() << "Draw time: " << starttime.msecsTo(QTime::currentTime()) << "ms";

    return final;
}

bool SingleRender::left = true;
