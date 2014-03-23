#include "renderers.h"
#include <QWidget>
#include <QPainter>
#include <QBitmap>

void drawInterlaced(const QPixmap &imgL, const QPixmap &imgR, int panX, int panY, QPainter &painter, QBitmap &mask, qreal zoom){
    if(zoom <= 0.0){
        zoom = qMin(qreal(painter.window().width()) / qreal(imgL.width()), qreal(painter.window().height()) / qreal(imgL.height()));
    }
    QRect outRect(panX + painter.window().width()  / 2 - (imgL.width()  / 2) * zoom,
                  panY + painter.window().height() / 2 - (imgL.height() / 2) * zoom,
                  imgL.width() * zoom, imgL.height() * zoom);

    QRect maskArea = outRect & painter.window();

    painter.setClipRect(maskArea);

    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.drawRect(maskArea);

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.setBackgroundMode(Qt::TransparentMode);

    for(int x = maskArea.left(); x < maskArea.right(); x += mask.width()){
        for(int y = maskArea.top(); y < maskArea.bottom(); y += mask.height()){
            painter.drawPixmap(x, y, mask);
        }
    }

    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.drawPixmap(outRect, imgL);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    painter.drawPixmap(outRect, imgR);
}

