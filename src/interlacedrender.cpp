#include "renderers.h"
#include <QWidget>
#include <QPainter>
#include <QBitmap>

void drawInterlaced(const QImage &imgL, const QImage &imgR, int panX, int panY, QPainter &painter, QBitmap &mask, qreal zoom, QWidget *parent){
    if(zoom <= 0.0){
        zoom = qMin(qreal(painter.window().width()) / qreal(imgL.width()), qreal(painter.window().height()) / qreal(imgL.height()));
    }

    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.drawRect(painter.window());

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.setBackgroundMode(Qt::TransparentMode);

    // TODO - offset this based on widget's position on screen.
    for(int x = 0; x < painter.window().width(); x += mask.width()){
        for(int y = 0; y < painter.window().height(); y += mask.height()){
            painter.drawPixmap(x, y, mask);
        }
    }

    painter.translate(painter.window().width() / 2,
                      painter.window().height() / 2);

    painter.translate(panX, panY);

    painter.scale(zoom, zoom);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.drawImage(-imgL.width() / 2, -imgL.height() / 2, imgL);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    painter.drawImage(-imgL.width() / 2, -imgL.height() / 2, imgR);
}

