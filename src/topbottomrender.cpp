#include "renderers.h"

QImage drawTopBottom(const QImage &imgL, const QImage &imgR, int panX, int panY, QSize finalSize, float zoom, bool mirrorL, bool mirrorR){
    if(finalSize.isEmpty()){
        finalSize.setWidth(imgL.width());
        finalSize.setHeight(imgL.height() + imgR.height());
    }

    if(zoom <= 0.0f){
        zoom = qMin(float(finalSize.width()) / float(imgL.width()), float(finalSize.height()) / float(imgL.height()) * 0.5f);
    }

    panY *= 0.5f;
    panX += finalSize.width()  * 0.5f  - imgL.width()  * zoom * 0.5f;
    panY += finalSize.height() * 0.25f - imgL.height() * zoom * 0.5f;

    QImage final(finalSize, QImage::Format_RGB32);

    QRgb *lineOut;
    QRgb *lineIn;

    for(int y = 0; y < finalSize.height(); y++){
        lineOut = (QRgb*)final.scanLine(y);
        int cy = y;

        if(y > finalSize.height() / 2){
            if(mirrorR){
                cy = (finalSize.height() * 0.5f) - cy;
                cy += finalSize.height() / 2;
            }else{
                cy -= finalSize.height() / 2;
            }
            cy = (cy - panY) / zoom;

            if(cy >= 0 && cy < imgR.height()){
                lineIn = (QRgb*)imgR.constScanLine(cy);
            }

            for(int x = 0; x < finalSize.width(); x++){
                int cx = (x - panX) / zoom;

                if(imgR.valid(cx, cy)){
                    lineOut[x] = lineIn[cx];
                }else{
                    lineOut[x] = qRgb(0,0,0);
                }
            }
        }
        else if(y < finalSize.height() / 2){
            if(mirrorL){
                cy = (finalSize.height() * 0.5f) - cy;
            }
            cy = (cy - panY) / zoom;

            if(cy >= 0 && cy < imgL.height()){
                lineIn = (QRgb*)imgL.constScanLine(cy);
            }

            for(int x = 0; x < finalSize.width(); x++){
                int cx = (x - panX) / zoom;

                if(imgL.valid(cx, cy)){
                    lineOut[x] = lineIn[cx];
                }else{
                    lineOut[x] = qRgb(0,0,0);
                }
            }
        }else{
            for(int x = 0; x < finalSize.width(); x++){
                lineOut[x] = qRgb(0,0,0);
            }
        }
    }

    return final;
}

