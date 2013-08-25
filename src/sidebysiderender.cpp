#include "renderers.h"

QImage drawSideBySide(const QImage &imgL, const QImage &imgR, int panX, int panY, QSize finalSize, float zoom, bool mirrorL, bool mirrorR){
    if(finalSize.isEmpty()){
        finalSize.setWidth(imgL.width() + imgR.width());
        finalSize.setHeight(imgL.height());
    }

    if(zoom <= 0.0f){
        zoom = qMin(float(finalSize.width()) / float(imgL.width()) * 0.5f, float(finalSize.height()) / float(imgL.height()));
    }

    panX += finalSize.width() * 0.5f - imgL.width() * zoom;
    panX *= 0.5f;
    panY += (finalSize.height() - imgL.height() * zoom) * 0.5f;

    QImage final(finalSize, QImage::Format_RGB32);

    QRgb *line;
    QRgb *lineL;
    QRgb *lineR;

    int halfWidth = finalSize.width() / 2;

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
                    cxl = halfWidth - cxl;
                }
                if(mirrorR){
                    cxr = finalSize.width() - cxr;
                }else{
                    cxr -= halfWidth;
                }
                cxl = (cxl - panX) / zoom;
                cxr = (cxr - panX) / zoom;

                if(x > halfWidth && imgR.valid(cxr,cy)){
                    line[x]=lineR[cxr];
                }else if(x < halfWidth && imgL.valid(cxl,cy)){
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

