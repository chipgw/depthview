#include "renderers.h"

QImage drawSingle(const QImage &img, int panX, int panY, QSize finalSize, float zoom){
    if(finalSize.isEmpty()){
        finalSize = img.size();
    }

    QImage in;
    if(zoom == 0.0f){
        in = img.scaled(finalSize, Qt::KeepAspectRatio);
    }else{
        in = img.scaled(img.size() * zoom, Qt::KeepAspectRatio);
    }

    panX += (finalSize.width()  - in.width())  * 0.5f;
    panY += (finalSize.height() - in.height()) * 0.5f;

    QImage final = in.copy(-panX, -panY, finalSize.width(), finalSize.height());

    return final;
}

