#include "sidebysiderender.h"

SideBySideRender::SideBySideRender(){
}

QImage SideBySideRender::draw(const QImage &imgL, const QImage &imgR, int panX, int panY, int finalwidth, int finalheight, float zoom){
    QTime starttime = QTime::currentTime();

    if(finalwidth <= 0 || finalheight <= 0){
        finalwidth = imgL.width();
        finalheight = imgL.height();
    }

    if(zoom <= 0.0f){
        zoom = qMin((float)finalwidth / (float)imgL.width(), (float)finalheight / (float)imgL.height());
    }
    // to make the zoom work right.
    zoom *= 0.5f;

    panX *= 0.5f;
    panX += finalwidth * 0.25f - imgL.width()  * zoom * 0.5f;
    panY += finalheight * 0.5f - imgL.height() * zoom * 0.5f;

    QImage final(finalwidth,finalheight,QImage::Format_RGB32);

    QRgb *line;
    QRgb *lineL;
    QRgb *lineR;

    for(int y=0;y<finalheight;y++){
        line = (QRgb *)final.scanLine(y);
        int cy = (y-panY)/zoom;
        if(cy >= 0 && cy < imgL.height()){
            lineL = (QRgb *)imgL.constScanLine(cy);
            lineR = (QRgb *)imgR.constScanLine(cy);
            for(int x=0;x<finalwidth;x++){
                int cxl = x;
                int cxr = x;
                if(SideBySideRender::mirrorL){
                    cxl = (imgL.width() * 0.25f)-cxl;
                }
                if(SideBySideRender::mirrorR){
                    cxr = (imgL.width() * 0.25f)-cxr;
                    cxr += finalwidth/2;
                }else{
                    cxr -= finalwidth/2;
                }
                cxl = (cxl-panX)/zoom;
                cxr = (cxr-panX)/zoom;

                if(x>finalwidth/2 && imgR.valid(cxr,cy)){
                    line[x]=lineR[cxr];
                }
                else if(x<finalwidth/2 && imgL.valid(cxl,cy)){
                    line[x]=lineL[cxl];
                }
                else{
                    line[x] = qRgb(0,0,0);
                }
            }
        }else{
            for(int x=0;x<final.width();x++)
                line[x] = qRgb(0,0,0);
        }
    }

    qDebug() << "Draw time: " << starttime.msecsTo(QTime::currentTime()) << "ms";

    return final;
}

bool SideBySideRender::mirrorL = false;
bool SideBySideRender::mirrorR = false;
