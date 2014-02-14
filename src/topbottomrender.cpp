#include "renderers.h"

QImage drawTopBottom(const QImage &imgL, const QImage &imgR, int panX, int panY, QSize finalSize, float zoom, bool mirrorL, bool mirrorR){
    if(finalSize.isEmpty()){
        finalSize.setWidth(imgL.width());
        finalSize.setHeight(imgL.height() + imgR.height());
    }

    if(zoom <= 0.0f){
        zoom = qMin(float(finalSize.width()) / float(imgL.width()), float(finalSize.height()) / float(imgL.height()) * 0.5f);
    }

    panX += (finalSize.width() - imgL.width() * zoom) * 0.5f;
    panY += finalSize.height() * 0.5f - imgL.height() * zoom;
    panY *= 0.5f;

    QImage final(finalSize, QImage::Format_RGB32);

    QRgb *lineOut;
    const QRgb *lineIn;

    int rightOffset;
    int halfHeight = finalSize.height() / 2;
    if(mirrorR){
        rightOffset = finalSize.height();
    }else{
        rightOffset = -halfHeight;
    }

    for(int y = 0; y < finalSize.height(); ++y){
        lineOut = (QRgb*)final.scanLine(y);
        if(y > halfHeight){
            int cy = y;
            if(mirrorR){
                cy = -cy;
            }
            cy = (rightOffset + cy - panY) / zoom;

            if(cy >= 0 && cy < imgR.height()){
                lineIn = (const QRgb*)imgR.constScanLine(cy);
            }

            for(int x = 0; x < finalSize.width(); ++x){
                int cx = (x - panX) / zoom;

                if(imgR.valid(cx, cy)){
                    lineOut[x] = lineIn[cx];
                }else{
                    lineOut[x] = 0;
                }
            }
        }else if(y < halfHeight){
            int cy = y;
            if(mirrorL){
                cy = halfHeight - cy;
            }
            cy = (cy - panY) / zoom;

            if(cy >= 0 && cy < imgL.height()){
                lineIn = (const QRgb*)imgL.constScanLine(cy);
            }

            for(int x = 0; x < finalSize.width(); ++x){
                int cx = (x - panX) / zoom;

                if(imgL.valid(cx, cy)){
                    lineOut[x] = lineIn[cx];
                }else{
                    lineOut[x] = 0;
                }
            }
        }else{
            for(int x = 0; x < finalSize.width(); ++x){
                lineOut[x] = 0;
            }
        }
    }

    return final;
}

