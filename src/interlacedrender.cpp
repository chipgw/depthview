#include "renderers.h"
#include <QWidget>

QImage drawInterlaced(const QImage &imgL, const QImage &imgR, int panX, int panY, QSize finalSize, float zoom, bool horizontal, QWidget *parent){
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

    if(zoom <= 0.0f){
        zoom = qMin(float(finalSize.width()) / float(imgL.width()), float(finalSize.height()) / float(imgL.height()));
    }

    panX += (finalSize.width()  - imgL.width()  * zoom) * 0.5f;
    panY += (finalSize.height() - imgL.height() * zoom) * 0.5f;

    QImage final(finalSize, QImage::Format_RGB32);

    QRgb *line;

    if(horizontal){
        QRgb *lineIn = NULL;

        for(int y = 0; y < final.height(); ++y){
            line = (QRgb*)final.scanLine(y);
            int cy = (y - panY) / zoom;

            if(y % 2 == first){
                if(cy >= 0 && cy < imgR.height()){
                    lineIn = (QRgb*)imgR.constScanLine(cy);
                }
            }else{
                if(cy >= 0 && cy < imgL.height()){
                    lineIn = (QRgb*)imgL.constScanLine(cy);
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

