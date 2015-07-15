#include "renderers.h"
#include <QPainter>

void drawTopBottom(const QPixmap &imgL, const QPixmap &imgR, int panX, int panY, QPainter &painter, bool anamorphic, qreal zoom, bool mirrorL, bool mirrorR){
    QRect size = painter.window();

    /* Calculate the zoom based on how the image will fit in the painter window. */
    if(zoom <= 0.0)
        zoom = qMin(qreal(size.width()) / qreal(imgL.width()), qreal(size.height()) / qreal(imgL.height()) * (anamorphic ? 1.0 : 0.5));

    QRect clip(0, 0, size.width(), size.height() / 2);

    painter.setClipRect(clip);

    painter.translate(size.width() / 2,
                      size.height() / 4);

    if(mirrorL) painter.scale(1.0, -1.0);

    /* If anamorphic is enabled we ned to squash the y-axis. */
    if(anamorphic) painter.scale(1.0, 0.5);

    painter.translate(panX, panY);

    painter.scale(zoom, zoom);
    painter.drawPixmap(-imgL.width() / 2, -imgL.height() / 2, imgL);

    painter.resetTransform();
    painter.translate(0, size.height() / 2);

    painter.setClipRect(clip);

    painter.translate(size.width() / 2,
                      size.height() / 4);

    if(mirrorR) painter.scale(1.0, -1.0);

    /* If anamorphic is enabled we ned to squash the y-axis. */
    if(anamorphic) painter.scale(1.0, 0.5);

    painter.translate(panX, panY);

    painter.scale(zoom, zoom);
    painter.drawPixmap(-imgR.width() / 2, -imgR.height() / 2, imgR);
}

