#include "renderers.h"

QImage drawSingle(const QImage &img, int panX, int panY, QSize finalSize, float zoom){
    if(finalSize.isEmpty()){
        finalSize = img.size();
    }

    QImage in;
    if(zoom == 0.0f){
        in = img.scaled(finalSize, Qt::KeepAspectRatio);
    }else{
        in = img.scaled(img.width() * zoom, img.height() * zoom, Qt::KeepAspectRatio);
    }

    panX += finalSize.width()  * 0.5f - in.width()  * 0.5f;
    panY += finalSize.height() * 0.5f - in.height() * 0.5f;

    QImage final = in.copy(-panX, -panY, finalSize.width(), finalSize.height());

    return final;
}

