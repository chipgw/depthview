#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QScrollBar>
#include <QTimer>

class ImageWidget : public QWidget {
    Q_OBJECT

protected:
    QScrollBar hBar;
    QScrollBar vBar;
    QPoint lastmousepos;
    float zoom;

    QTimer mouseTimer;

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
    const static QMap<QString, DrawMode> drawModeNames;

    QImage imgL, imgR;
    bool swapLR;
    bool showScrollbars;
    bool enableContinuousPan;
    DrawMode mode;

    explicit ImageWidget(QWidget *parent = 0);
    bool loadStereoImage(const QString &filename);
    void setZoom(float val);
    void addZoom(float amount);

public slots:
    void hideCursor();
    void zoomIn();
    void zoomOut();
    void recalculatescroolmax();

signals:
    void doubleClicked();

protected:
    QImage draw(const QImage &L, const QImage &R);

    void resizeEvent(QResizeEvent * e);
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
};

#endif // IMAGEWIDGET_H
