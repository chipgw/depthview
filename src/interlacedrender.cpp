#include "renderers.h"
#include <QWidget>
#include <QPainter>
#include <QBitmap>

void drawInterlaced(const QPixmap &imgL, const QPixmap &imgR, int panX, int panY, QPainter &painter, QBitmap &mask, qreal zoom){
    if(zoom <= 0.0)
        zoom = qMin(qreal(painter.window().width()) / qreal(imgL.width()), qreal(painter.window().height()) / qreal(imgL.height()));

    QRect outRect(panX + painter.window().width()  / 2 - (imgL.width()  / 2) * zoom,
                  panY + painter.window().height() / 2 - (imgL.height() / 2) * zoom,
                  imgL.width() * zoom, imgL.height() * zoom);

    /* The area where the outRect and painter window intersect. */
    QRect maskArea = outRect & painter.window();

    painter.setClipRect(maskArea);

    /* Clear the area to draw the image. */
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.drawRect(maskArea);

    /* Set the painter to draw black pixels as transparent and white as opaque. */
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.setBackgroundMode(Qt::TransparentMode);

    /* Fill the image area with the mask. */
    for(int x = maskArea.left(); x < maskArea.right(); x += mask.width())
        for(int y = maskArea.top(); y < maskArea.bottom(); y += mask.height())
            painter.drawPixmap(x, y, mask);

    /* Draw left image in opaque pixels. */
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.drawPixmap(outRect, imgL);

    /* Draw right image in transparent pixels. */
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    painter.drawPixmap(outRect, imgR);
}

