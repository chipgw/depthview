#pragma once

#include <QImage>
#include <QPixmap>

/* Anaglyph generates a pixmap once on loading the image and uses that with drawSingle() for zooming & panning. */
QImage drawAnaglyph(const QImage &imgL, const QImage &imgR);
QImage drawAnaglyphHalf(const QImage &imgL, const QImage &imgR);
QImage drawAnaglyphGrey(const QImage &imgL, const QImage &imgR);

/* Interlaced/checkerboard uses a mask to tell which image to use for which screen pixels. Only works properly without the menubar in fullscreen mode. */
void drawInterlaced(const QPixmap &imgL, const QPixmap &imgR, int panX, int panY, QPainter &painter, QBitmap &mask, qreal zoom = 0.0);

/* These draw the two images in seperate halves of the screen, panning so that the same area is shown on each half regardless of mirroring. */
void drawSideBySide(const QPixmap &imgL, const QPixmap &imgR, int panX, int panY, QPainter &painter, bool anamorphic, qreal zoom = 0.0, bool mirrorL = false, bool mirrorR = false);
void drawTopBottom(const QPixmap &imgL, const QPixmap &imgR, int panX, int panY, QPainter &painter, bool anamorphic, qreal zoom = 0.0, bool mirrorL = false, bool mirrorR = false);

/* Zoom and pan a single image, no fance magic involved. */
void drawSingle(const QPixmap &img, int panX, int panY, QPainter &painter, qreal zoom = 0.0);
