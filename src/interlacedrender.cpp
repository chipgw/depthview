#include "renderers.h"
#include <QWidget>
#include <QPainter>
#include <QBitmap>

void drawInterlaced(const QPixmap &imgL, const QPixmap &imgR, int panX, int panY, QPainter &painter, QBitmap &mask, qreal zoom, QWidget *parent){
    if(zoom <= 0.0){
        zoom = qMin(qreal(painter.window().width()) / qreal(imgL.width()), qreal(painter.window().height()) / qreal(imgL.height()));
    }
    QRect outRect(panX + painter.window().width()  / 2 - (imgL.width()  / 2) * zoom,
                  panY + painter.window().height() / 2 - (imgL.height() / 2) * zoom,
                  imgL.width() * zoom, imgL.height() * zoom);

    painter.setClipRect(outRect);

    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.drawRect(painter.window());

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.setBackgroundMode(Qt::TransparentMode);

    // TODO - offset this based on widget's position on screen.
    for(int x = outRect.left(); x < outRect.right(); x += mask.width()){
        for(int y = outRect.top(); y < outRect.bottom(); y += mask.height()){
            painter.drawPixmap(x, y, mask);
        }
    }


    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.drawPixmap(outRect, imgL);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    painter.drawPixmap(outRect, imgR);
}

