#include "renderers.h"
#include <QPainter>

void drawSideBySide(const QImage &imgL, const QImage &imgR, int panX, int panY, QPainter &painter, float zoom, bool mirrorL, bool mirrorR){
    QRect size = painter.window();

    if(zoom <= 0.0f){
        zoom = qMin(float(size.width()) / float(imgL.width()) * 0.5f, float(size.height()) / float(imgL.height()));
    }

    QRect clip(0, 0, size.width() / 2, size.height());

    painter.setClipRect(clip);

    painter.translate(size.width() / 4,
                      size.height() / 2);

    if(mirrorL) painter.scale(-1, 1);

    painter.translate(panX / 2, panY);

    painter.scale(zoom, zoom);
    painter.drawImage(-imgL.width() / 2, -imgL.height() / 2, imgL);

    painter.resetTransform();
    painter.translate(size.width() / 2, 0);

    painter.setClipRect(clip);

    painter.translate(size.width() / 4,
                      size.height() / 2);

    if(mirrorR) painter.scale(-1, 1);

    painter.translate(panX / 2, panY);

    painter.scale(zoom, zoom);
    painter.drawImage(-imgR.width() / 2, -imgR.height() / 2, imgR);
}

