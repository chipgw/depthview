#include "renderers.h"
#include <QTime>
#include <QDebug>

QImage drawSingle(const QImage &img, int panX, int panY, int finalwidth, int finalheight, float zoom){
    QTime starttime = QTime::currentTime();

    if(finalwidth <= 0 || finalheight <= 0){
        finalwidth  = img.width();
        finalheight = img.height();
    }

    QImage in;
    if(zoom == 0.0f){
        in = img.scaled(finalwidth,finalheight, Qt::KeepAspectRatio);
    }
    else{
        in = img.scaled(img.width() * zoom, img.height() * zoom, Qt::KeepAspectRatio);
    }

    panX += finalwidth  * 0.5f - in.width()  * 0.5f;
    panY += finalheight * 0.5f - in.height() * 0.5f;

    QImage final = in.copy(-panX, -panY, finalwidth, finalheight);

    qDebug() << "Draw time: " << starttime.msecsTo(QTime::currentTime()) << "ms";

    return final;
}

