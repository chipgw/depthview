#include "renderers.h"
#include <QWidget>

QImage drawInterlaced(const QImage &imgL, const QImage &imgR, int panX, int panY, QSize finalSize, qreal zoom, bool horizontal, QWidget *parent){
    bool first = 1;
    if(parent){
        if(horizontal){
            first = parent->pos().x() % 2;
        }else{
            first = parent->pos().y() % 2;
        }
    }
    if(finalSize.isEmpty()){
        finalSize = imgL.size();
    }

    if(zoom <= 0.0){
        zoom = qMin(qreal(finalSize.width()) / qreal(imgL.width()), qreal(finalSize.height()) / qreal(imgL.height()));
    }

    panX += (finalSize.width()  - imgL.width()  * zoom) * 0.5;
    panY += (finalSize.height() - imgL.height() * zoom) * 0.5;

    QImage final(finalSize, QImage::Format_RGB32);

    QRgb *line;

    if(horizontal){
        const QRgb *lineIn;

        for(int y = 0; y < final.height(); ++y){
            line = (QRgb*)final.scanLine(y);
            int cy = (y - panY) / zoom;

            if(y % 2 == first){
                if(cy >= 0 && cy < imgR.height()){
                    lineIn = (const QRgb*)imgR.constScanLine(cy);
                }
            }else{
                if(cy >= 0 && cy < imgL.height()){
                    lineIn = (const QRgb*)imgL.constScanLine(cy);
                }
            }

            for(int x = 0; x < final.width(); ++x){
                int cx = (x - panX) / zoom;
                if(imgL.valid(cx, cy) && imgR.valid(cx, cy)){
                    line[x] = lineIn[cx];
                }else{
                    line[x] = 0;
                }
            }
        }
    }else{
        const QRgb *lineL;
        const QRgb *lineR;

        for(int y = 0; y < final.height(); ++y){
            line = (QRgb*)final.scanLine(y);
            int cy = (y - panY) / zoom;
            if(cy >= 0 && cy < imgL.height()){
                lineL = (const QRgb*)imgL.constScanLine(cy);
                lineR = (const QRgb*)imgR.constScanLine(cy);
                for(int x = 0; x < final.width(); ++x){
                    int cx = (x - panX) / zoom;
                    if(imgL.valid(cx, cy) && imgR.valid(cx, cy)){
                        if(x % 2 == first){
                            line[x] = lineR[cx];
                        }else{
                            line[x] = lineL[cx];
                        }
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
    }

    return final;
}

