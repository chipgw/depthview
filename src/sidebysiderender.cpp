#include "renderers.h"
#include <QPainter>

void drawSideBySide(const QPixmap &imgL, const QPixmap &imgR, int panX, int panY, QPainter &painter, qreal zoom, bool mirrorL, bool mirrorR){
    QRect size = painter.window();

    if(zoom <= 0.0){
        zoom = qMin(qreal(size.width()) / qreal(imgL.width()) * 0.5, qreal(size.height()) / qreal(imgL.height()));
    }

    QRect clip(0, 0, size.width() / 2, size.height());

    painter.setClipRect(clip);

    painter.translate(size.width() / 4,
                      size.height() / 2);

    if(mirrorL) painter.scale(-1, 1);

    painter.translate(panX, panY);

    painter.scale(zoom, zoom);
    painter.drawPixmap(-imgL.width() / 2, -imgL.height() / 2, imgL);

    painter.resetTransform();
    painter.translate(size.width() / 2, 0);

    painter.setClipRect(clip);

    painter.translate(size.width() / 4,
                      size.height() / 2);

    if(mirrorR) painter.scale(-1, 1);

    painter.translate(panX, panY);

    painter.scale(zoom, zoom);
    painter.drawPixmap(-imgR.width() / 2, -imgR.height() / 2, imgR);
}

