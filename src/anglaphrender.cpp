#include "renderers.h"

const QRgb lEyeMask = qRgb(0xff, 0x00, 0x00);
const QRgb rEyeMask = qRgb(0x00, 0xff, 0xff);

QImage drawAnglaph(const QImage &imgL, const QImage &imgR){
    QImage final(imgL.size(), QImage::Format_RGB32);

    QRgb *line;
    const QRgb *lineL;
    const QRgb *lineR;

    for(int y = 0; y < final.height(); ++y){
        line = (QRgb*)final.scanLine(y);
        lineL = (const QRgb*)imgL.constScanLine(y);
        lineR = (const QRgb*)imgR.constScanLine(y);
        for(int x = 0; x < final.width(); ++x){
            if(imgL.valid(x,y) && imgR.valid(x,y)){
                line[x] = (lineL[x] & lEyeMask) | (lineR[x] & rEyeMask);
            }else{
                line[x] = 0;
            }
        }
    }

    return final;
}

QImage drawAnglaphHalf(const QImage &imgL, const QImage &imgR){
    QImage final(imgL.size(), QImage::Format_RGB32);

    QRgb *line;
    const QRgb *lineL;
    const QRgb *lineR;

    for(int y = 0; y < final.height(); ++y){
        line = (QRgb*)final.scanLine(y);
        lineL = (const QRgb*)imgL.constScanLine(y);
        lineR = (const QRgb*)imgR.constScanLine(y);
        for(int x = 0; x < final.width(); ++x){
            if(imgL.valid(x,y) && imgR.valid(x,y)){
                int gL = qGray(lineL[x]) / 2;
                int gR = qGray(lineR[x]) / 2;
                line[x] = qRgb(qRed(  lineL[x]) / 2 + gL,
                               qGreen(lineR[x]) / 2 + gR,
                               qBlue( lineR[x]) / 2 + gR);
            }else{
                line[x] = 0;
            }
        }
    }

    return final;
}

QImage drawAnglaphGrey(const QImage &imgL, const QImage &imgR){
    QImage final(imgL.size(), QImage::Format_RGB32);

    QRgb *line;
    const QRgb *lineL;
    const QRgb *lineR;

    for(int y = 0; y < final.height(); ++y){
        line = (QRgb*)final.scanLine(y);
        lineL = (const QRgb*)imgL.constScanLine(y);
        lineR = (const QRgb*)imgR.constScanLine(y);
        for(int x = 0; x < final.width(); ++x){
            if(imgL.valid(x,y) && imgR.valid(x,y)){
                int gL = qGray(lineL[x]);
                int gR = qGray(lineR[x]);
                line[x] = qRgb(gL, gR, gR);
            }else{
                line[x] = 0;
            }
        }
    }

    return final;
}

