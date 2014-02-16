#include "renderers.h"
#include <QPainter>

void drawSideBySide(const QImage &imgL, const QImage &imgR, int panX, int panY, QPainter &painter, float zoom, bool mirrorL, bool mirrorR){
    if(zoom <= 0.0f){
        zoom = qMin(float(painter.window().width()) / float(imgL.width()) * 0.5f, float(painter.window().height()) / float(imgL.height()));
    }

    QRect clip(0, 0, painter.window().width() / 2, painter.window().height());

    painter.setClipRect(clip);

    painter.translate(painter.window().width() / 4,
                      painter.window().height() / 2);
    if(mirrorL) painter.scale(-1, 1);

    painter.translate(panX / 2, panY);

    painter.scale(zoom, zoom);
    painter.translate(-imgL.width() / 2, -imgR.height() / 2);
    painter.drawImage(0, 0, imgL);

    painter.resetTransform();

    painter.translate(painter.window().width() / 2, 0);
    painter.setClipRect(clip);
    painter.translate(painter.window().width() / 4,
                      painter.window().height() / 2);
    if(mirrorR) painter.scale(-1, 1);

    painter.translate(panX / 2, panY);

    painter.scale(zoom, zoom);
    painter.translate(-imgR.width() / 2, -imgR.height() / 2);
    painter.drawImage(0, 0, imgR);
}

