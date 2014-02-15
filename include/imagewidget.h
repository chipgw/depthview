#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QScrollBar>
#include <QTimer>


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
    QPoint lastmousepos;
    float zoom;
    bool swapLR;
    bool scrollbarsVisible;
    bool continuousPan;

    QTimer mouseTimer;

    DrawMode mode;

public:
    const static QMap<QString, DrawMode> drawModeNames;

    QImage imgL, imgR;

    explicit ImageWidget(QWidget *parent = NULL);
    bool loadStereoImage(const QString &filename);
    void setZoom(float val);
    void addZoom(float amount);

    void setRenderMode(DrawMode m);

public slots:
    void hideCursor();
    void zoomIn();
    void zoomOut();
    void recalculatescroolmax();

    void enableSwapLR(bool enable);
    void showScrollbars(bool show);
    void enableContinuousPan(bool enable);

protected:
    void draw(const QImage &L, const QImage &R, QPainter &painter);

    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
};

#endif // IMAGEWIDGET_H
