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
    const QRgb *lineL;
    const QRgb *lineR;

    int halfWidth = finalSize.width() / 2;

    for(int y = 0; y < finalSize.height(); ++y){
        line = (QRgb*)final.scanLine(y);
        int cy = (y - panY) / zoom;
        if(cy >= 0 && cy < imgL.height()){
            lineL = (const QRgb*)imgL.constScanLine(cy);
            lineR = (const QRgb*)imgR.constScanLine(cy);
            for(int x = 0; x < finalSize.width(); ++x){
                if(x > halfWidth){
                    int cx = x;
                    if(mirrorR){
                        cx = finalSize.width() - cx;
                    }else{
                        cx -= halfWidth;
                    }
                    cx = (cx - panX) / zoom;

                    if(imgR.valid(cx,cy)){
                        line[x]=lineR[cx];
                        continue;
                    }
                }else if(x < halfWidth){
                    int cx = x;
                    if(mirrorL){
                        cx = halfWidth - cx;
                    }
                    cx = (cx - panX) / zoom;

                    if(imgL.valid(cx,cy)){
                        line[x]=lineL[cx];
                        continue;
                    }
                }

                line[x] = 0;
            }
        }else{
            for(int x = 0; x < final.width(); ++x){
                line[x] = 0;
            }
        }
    }

    return final;
}

