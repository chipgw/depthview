#include "renderers.h"
#include <QWidget>

QImage drawCheckerboard(const QImage &imgL, const QImage &imgR, int panX, int panY, QSize finalSize, qreal zoom, QWidget *parent){
    if(zoom <= 0.0){
        zoom = qMin(qreal(finalSize.width()) / qreal(imgL.width()), qreal(finalSize.width()) / qreal(imgL.height()));
    }
    QPoint pos = parent->mapToGlobal(QPoint());

    panX += (finalSize.width()  - imgL.width()  * zoom) * 0.5;
    panY += (finalSize.height() - imgL.height() * zoom) * 0.5;

    QImage final(finalSize, QImage::Format_RGB32);

    QRgb *line;
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
                if(imgL.valid(cx,cy) && imgR.valid(cx,cy)){
                    if((pos.x() + x) % 2 == (pos.y() + y) % 2){
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

    return final;
}

