#pragma once

#include <QScrollBar>
#include <QTimer>
#include <QBitmap>

class ImageWidget : public QWidget {
    Q_OBJECT
public:
    enum DrawMode{
        AnglaphFull,
        AnglaphHalf,
        AnglaphGreyscale,
        SidebySide,
        SidebySideMLeft,
        SidebySideMRight,
        SidebySideMBoth,
        TopBottom,
        TopBottomMTop,
        TopBottomMBottom,
        TopBottomMBoth,
        InterlacedHorizontal,
        InterlacedVertical,
        Checkerboard,
        MonoLeft,
        MonoRight
    };

protected:
    QScrollBar hBar;
    QScrollBar vBar;

    /* The position of the mouse in the last mouse event. */
    QPoint lastMousePos;

    /* Zoom factor. 1.0 is 1:1, 0 is auto fit. */
    qreal zoom;

    /* Enable using imgL for the right eye and visa versa. */
    bool swapLR;

    /* Are the scrollbars visible? */
    bool scrollbarsVisible;

    /* Warp the mouse to the other end of the screen when it reaches the edge while panning. */
    bool continuousPan;

    /* Do we use smooth transformations with QPainter? */
    bool smoothTransform;

    /* The button mask used to check if a mouse movement event translates to panning. */
    Qt::MouseButtons panButtons;

    /* This timer hides the mouse 4 seconds after it was last moved. */
    QTimer mouseTimer;

    DrawMode mode;

    /* The pixmap generated for anglaph draw modes. */
    QPixmap anglaph;

    /* Interlacing masks. */
    QBitmap maskInterlacedHorizontal;
    QBitmap maskInterlacedVertical;
    QBitmap maskCheckerboard;

public:
    /* Strings to use for text serialization of the DrawMode enum. */
    const static QMap<QString, DrawMode> drawModeNames;

    /* The left and right eye images. */
    QImage imgL, imgR;
    QPixmap pixmapL, pixmapR;

    explicit ImageWidget(QWidget *parent = NULL);

    /* Load a .jps or .pns file from the hard disk. Returns true on success. */
    bool loadStereoImage(const QString &filename);

    /* Zoom functions. */
    void setZoom(qreal val);
    void addZoom(qreal amount);

    /* Change the draw mode. Will recalculate scroll bounds, update anglaph pixmap (if needed), and repaint the widget. */
    void setRenderMode(DrawMode m);

    /* Set the mouse button mask used for panning. */
    void setPanButtons(Qt::MouseButtons buttons);

    /* Call this after modifying imgL and/or imgR. */
    void updateImages();

public slots:
    /* What it says. */
    void hideCursor();
    void zoomIn();
    void zoomOut();

    /* Recalculates the bounds of the scrollbars to allow panning only where the image is drawn. */
    void recalculatescroolmax();

    /* Enable using imgL for the right eye and visa versa. */
    void enableSwapLR(bool enable);

    /* Show or hide the scrollbars. */
    void showScrollbars(bool show);

    /* Enables warping the mouse to the other end of the screen when it reaches the edge while panning. */
    void enableContinuousPan(bool enable);

    /* Enables the QPainter::SmoothPixmapTransform flag when painting, which decreases pixelation when zoom isn't 1:1. */
    void enableSmoothTransform(bool enable);

protected:
    /* Update the anglaph pixmaps. Should be called any time the images or the draw mode are changed. */
    void updateAnglaph();

    /* All the events we listen for. */
    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);

    void enterEvent(QEvent* e);
    void leaveEvent(QEvent* e);
};
