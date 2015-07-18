#include "renderers.h"
#include <QPainter>

void drawSingle(const QPixmap &img, int panX, int panY, QPainter &painter, qreal zoom){
    if(zoom <= 0.0)
        zoom = qMin(qreal(painter.window().width()) / qreal(img.width()), qreal(painter.window().height()) / qreal(img.height()));

    painter.translate(painter.window().width() / 2,
                      painter.window().height() / 2);

    painter.translate(panX, panY);

    painter.scale(zoom, zoom);
    painter.drawPixmap(-img.width() / 2, -img.height() / 2, img);
}

