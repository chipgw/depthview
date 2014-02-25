#include "renderers.h"
#include <QPainter>

void drawSingle(const QImage &img, int panX, int panY, QPainter &painter, float zoom){
    if(zoom <= 0.0f){
        zoom = qMin(float(painter.window().width()) / float(img.width()), float(painter.window().height()) / float(img.height()));
    }
    painter.translate(painter.window().width() / 2,
                      painter.window().height() / 2);

    painter.translate(panX, panY);

    painter.scale(zoom, zoom);
    painter.drawImage(-img.width() / 2, -img.height() / 2, img);
}

