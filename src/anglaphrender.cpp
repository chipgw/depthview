#include "renderers.h"
#include <QTime>
#include <QDebug>

QImage drawAnglaph(const QImage &imgL, const QImage &imgR, int panX, int panY, int finalwidth, int finalheight, float zoom, float colormult, QRgb colorL, QRgb colorR){
    QTime starttime = QTime::currentTime();

    if(finalwidth <= 0 || finalheight <= 0){
        finalwidth = imgL.width();
        finalheight = imgL.height();
    }

    if(zoom <= 0.0f){
        zoom = qMin(float(finalwidth) / float(imgL.width()), float(finalheight) / float(imgL.height()));
    }

    panX += (finalwidth  * 0.5f - imgL.width()  * zoom * 0.5f);
    panY += (finalheight * 0.5f - imgL.height() * zoom * 0.5f);

    QImage final(finalwidth, finalheight, QImage::Format_RGB32);

    QRgb *line;
    QRgb *lineL;
    QRgb *lineR;

    float redL   = qRed(colorL)   / 255.0f;
    float redR   = qRed(colorR)   / 255.0f;
    float greenL = qGreen(colorL) / 255.0f;
    float greenR = qGreen(colorR) / 255.0f;
    float blueL  = qBlue(colorL)  / 255.0f;
    float blueR  = qBlue(colorR)  / 255.0f;

    float greymult = 1.0f - colormult;

    for(int y = 0; y < final.height(); y++){
        line = (QRgb *)final.scanLine(y);
        int cy = (y - panY) / zoom;
        if(cy >= 0 && cy < imgL.height()){
            lineL = (QRgb *)imgL.constScanLine(cy);
            lineR = (QRgb *)imgR.constScanLine(cy);
            for(int x = 0; x < final.width(); x++){
                int cx = (x - panX) / zoom;
                if(imgL.valid(cx,cy) && imgR.valid(cx,cy)){
                    int gL = qGray(lineL[cx]) * greymult;
                    int gR = qGray(lineR[cx]) * greymult;
                    line[x] = qRgb((qRed(  lineL[cx]) * colormult + gL) * redL +
                                   (qRed(  lineR[cx]) * colormult + gR) * redR,
                                   (qGreen(lineL[cx]) * colormult + gL) * greenL +
                                   (qGreen(lineR[cx]) * colormult + gR) * greenR,
                                   (qBlue( lineL[cx]) * colormult + gL) * blueL +
                                   (qBlue( lineR[cx]) * colormult + gR) * blueR);
                }
                else{
                    line[x] = 0;
                }
            }
        }
        else{
            for(int x = 0; x < final.width(); x++)
                line[x] = 0;
        }
    }

    qDebug() << "Draw time: " << starttime.msecsTo(QTime::currentTime()) << "ms";

    return final;
}

