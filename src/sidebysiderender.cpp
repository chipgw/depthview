#include "renderers.h"

QImage drawSideBySide(const QImage &imgL, const QImage &imgR, int panX, int panY, QSize finalSize, float zoom, bool mirrorL, bool mirrorR){
    if(finalSize.isEmpty()){
        finalSize.setWidth(imgL.width() + imgR.width());
        finalSize.setHeight(imgL.height());
    }

    if(zoom <= 0.0f){
        zoom = qMin(float(finalSize.width()) / float(imgL.width()) * 0.5f, float(finalSize.height()) / float(imgL.height()));
    }

    panX *= 0.5f;
    panX += finalSize.width() * 0.25f - imgL.width()  * zoom * 0.5f;
    panY += finalSize.height() * 0.5f - imgL.height() * zoom * 0.5f;

    QImage final(finalSize, QImage::Format_RGB32);

    QRgb *line;
    QRgb *lineL;
    QRgb *lineR;

    for(int y = 0; y < finalSize.height(); y++){
        line = (QRgb*)final.scanLine(y);
        int cy = (y - panY) / zoom;
        if(cy >= 0 && cy < imgL.height()){
            lineL = (QRgb*)imgL.constScanLine(cy);
            lineR = (QRgb*)imgR.constScanLine(cy);
            for(int x = 0; x < finalSize.width(); x++){
                int cxl = x;
                int cxr = x;
                if(mirrorL){
                    cxl = (finalSize.width() * 0.5f) - cxl;
                }
                if(mirrorR){
                    cxr = (finalSize.width() * 0.5f) - cxr;
                    cxr += finalSize.width() / 2;
                }else{
                    cxr -= finalSize.width() / 2;
                }
                cxl = (cxl - panX) / zoom;
                cxr = (cxr - panX) / zoom;

                if(x > finalSize.width() / 2 && imgR.valid(cxr,cy)){
                    line[x]=lineR[cxr];
                }else if(x < finalSize.width() / 2 && imgL.valid(cxl,cy)){
                    line[x]=lineL[cxl];
                }else{
                    line[x] = qRgb(0,0,0);
                }
            }
        }else{
            for(int x = 0; x < final.width(); x++){
                line[x] = qRgb(0,0,0);
            }
        }
    }

    return final;
}

