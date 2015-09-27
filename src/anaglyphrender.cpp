#include "renderers.h"

const QRgb lEyeMask = qRgb(0xff, 0x00, 0x00);
const QRgb rEyeMask = qRgb(0x00, 0xff, 0xff);

QImage drawAnaglyph(const QImage &imgL, const QImage &imgR){
    QImage final(imgL.size(), QImage::Format_RGB32);

    QRgb *line;
    const QRgb *lineL;
    const QRgb *lineR;

    for(int y = 0; y < final.height(); ++y){
        /* Line to write to. */
        line = (QRgb*)final.scanLine(y);

        /* Lines to read from. */
        lineL = (const QRgb*)imgL.constScanLine(y);
        lineR = (const QRgb*)imgR.constScanLine(y);

        for(int x = 0; x < final.width(); ++x){
            if(imgL.valid(x,y) && imgR.valid(x,y))
                line[x] = (lineL[x] & lEyeMask) | (lineR[x] & rEyeMask);
            else
                /* We need to set all other pixels to 0 or else there will be garbage. */
                line[x] = 0;
        }
    }

    return final;
}

QImage drawAnaglyphHalf(const QImage &imgL, const QImage &imgR){
    QImage final(imgL.size(), QImage::Format_RGB32);

    QRgb *line;
    const QRgb *lineL;
    const QRgb *lineR;

    for(int y = 0; y < final.height(); ++y){
        /* Line to write to. */
        line = (QRgb*)final.scanLine(y);

        /* Lines to read from. */
        lineL = (const QRgb*)imgL.constScanLine(y);
        lineR = (const QRgb*)imgR.constScanLine(y);

        for(int x = 0; x < final.width(); ++x){
            if(imgL.valid(x,y) && imgR.valid(x,y)){
                /* Mix the greyscale values with the color values. */
                int gL = qGray(lineL[x]) / 2;
                int gR = qGray(lineR[x]) / 2;
                line[x] = qRgb(qRed(  lineL[x]) / 2 + gL,
                               qGreen(lineR[x]) / 2 + gR,
                               qBlue( lineR[x]) / 2 + gR);
            }else{
                /* We need to set all other pixels to 0 or else there will be garbage. */
                line[x] = 0;
            }
        }
    }

    return final;
}

QImage drawAnaglyphGrey(const QImage &imgL, const QImage &imgR){
    QImage final(imgL.size(), QImage::Format_RGB32);

    QRgb *line;
    const QRgb *lineL;
    const QRgb *lineR;

    for(int y = 0; y < final.height(); ++y){
        /* Line to write to. */
        line = (QRgb*)final.scanLine(y);

        /* Lines to read from. */
        lineL = (const QRgb*)imgL.constScanLine(y);
        lineR = (const QRgb*)imgR.constScanLine(y);

        for(int x = 0; x < final.width(); ++x){
            if(imgL.valid(x,y) && imgR.valid(x,y)){
                int gL = qGray(lineL[x]);
                int gR = qGray(lineR[x]);
                line[x] = qRgb(gL, gR, gR);
            }else{
                /* We need to set all other pixels to 0 or else there will be garbage. */
                line[x] = 0;
            }
        }
    }

    return final;
}

