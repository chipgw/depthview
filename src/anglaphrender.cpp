#include "renderers.h"

const QRgb lEyeMask = qRgb(0xff, 0x00, 0x00);
const QRgb rEyeMask = qRgb(0x00, 0xff, 0xff);

QImage drawAnglaph(const QImage &imgL, const QImage &imgR, int panX, int panY, QSize finalSize, float zoom){
    if(finalSize.isEmpty()){
        finalSize = imgL.size();
    }

    if(zoom <= 0.0f){
        zoom = qMin(float(finalSize.width()) / float(imgL.width()), float(finalSize.height()) / float(imgL.height()));
    }

    panX += (finalSize.width()  - imgL.width()  * zoom) * 0.5f;
    panY += (finalSize.height() - imgL.height() * zoom) * 0.5f;

    QImage final(finalSize, QImage::Format_RGB32);

    QRgb *line;
    QRgb *lineL;
    QRgb *lineR;

    for(int y = 0; y < final.height(); ++y){
        line = (QRgb*)final.scanLine(y);
        int cy = (y - panY) / zoom;
        if(cy >= 0 && cy < imgL.height()){
            lineL = (QRgb*)imgL.constScanLine(cy);
            lineR = (QRgb*)imgR.constScanLine(cy);
            for(int x = 0; x < final.width(); ++x){
                int cx = (x - panX) / zoom;
                if(imgL.valid(cx,cy) && imgR.valid(cx,cy)){
                    line[x] = (lineL[cx] & lEyeMask) | (lineR[cx] & rEyeMask);
                }else{
                    line[x] = 0;
                }
            }
        }else{
            for(int x = 0; x < final.width(); ++x){
                line[x] = 0;
            }
        }
    }

    return final;
}

QImage drawAnglaphHalf(const QImage &imgL, const QImage &imgR, int panX, int panY, QSize finalSize, float zoom){
    if(finalSize.isEmpty()){
        finalSize = imgL.size();
    }

    if(zoom <= 0.0f){
        zoom = qMin(float(finalSize.width()) / float(imgL.width()), float(finalSize.height()) / float(imgL.height()));
    }

    panX += (finalSize.width()  - imgL.width()  * zoom) * 0.5f;
    panY += (finalSize.height() - imgL.height() * zoom) * 0.5f;

    QImage final(finalSize, QImage::Format_RGB32);

    QRgb *line;
    QRgb *lineL;
    QRgb *lineR;

    for(int y = 0; y < final.height(); ++y){
        line = (QRgb*)final.scanLine(y);
        int cy = (y - panY) / zoom;
        if(cy >= 0 && cy < imgL.height()){
            lineL = (QRgb*)imgL.constScanLine(cy);
            lineR = (QRgb*)imgR.constScanLine(cy);
            for(int x = 0; x < final.width(); ++x){
                int cx = (x - panX) / zoom;
                if(imgL.valid(cx,cy) && imgR.valid(cx,cy)){
                    int gL = qGray(lineL[cx]) / 2;
                    int gR = qGray(lineR[cx]) / 2;
                    line[x] = qRgb(qRed(  lineL[cx]) / 2 + gL,
                                   qGreen(lineR[cx]) / 2 + gR,
                                   qBlue( lineR[cx]) / 2 + gR);
                }else{
                    line[x] = 0;
                }
            }
        }else{
            for(int x = 0; x < final.width(); ++x){
                line[x] = 0;
            }
        }
    }

    return final;
}

QImage drawAnglaphGrey(const QImage &imgL, const QImage &imgR, int panX, int panY, QSize finalSize, float zoom){
    if(finalSize.isEmpty()){
        finalSize = imgL.size();
    }

    if(zoom <= 0.0f){
        zoom = qMin(float(finalSize.width()) / float(imgL.width()), float(finalSize.height()) / float(imgL.height()));
    }

    panX += (finalSize.width()  - imgL.width()  * zoom) * 0.5f;
    panY += (finalSize.height() - imgL.height() * zoom) * 0.5f;

    QImage final(finalSize, QImage::Format_RGB32);

    QRgb *line;
    QRgb *lineL;
    QRgb *lineR;

    for(int y = 0; y < final.height(); ++y){
        line = (QRgb*)final.scanLine(y);
        int cy = (y - panY) / zoom;
        if(cy >= 0 && cy < imgL.height()){
            lineL = (QRgb*)imgL.constScanLine(cy);
            lineR = (QRgb*)imgR.constScanLine(cy);
            for(int x = 0; x < final.width(); ++x){
                int cx = (x - panX) / zoom;
                if(imgL.valid(cx,cy) && imgR.valid(cx,cy)){
                    int gL = qGray(lineL[cx]);
                    int gR = qGray(lineR[cx]);
                    line[x] = qRgb(gL, gR, gR);
                }else{
                    line[x] = 0;
                }
            }
        }else{
            for(int x = 0; x < final.width(); ++x){
                line[x] = 0;
            }
        }
    }

    return final;
}

