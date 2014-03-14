#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

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
    QPoint lastmousepos;
    qreal zoom;
    bool swapLR;
    bool scrollbarsVisible;
    bool continuousPan;
    Qt::MouseButtons panButtons;

    QTimer mouseTimer;

    DrawMode mode;

    QImage anglaph;

    QBitmap maskInterlacedHorizontal;
    QBitmap maskInterlacedVertical;
    QBitmap maskCheckerboard;

public:
    const static QMap<QString, DrawMode> drawModeNames;

    QImage imgL, imgR;

    explicit ImageWidget(QWidget *parent = NULL);
    bool loadStereoImage(const QString &filename);
    void setZoom(qreal val);
    void addZoom(qreal amount);

    void setRenderMode(DrawMode m);

    void setPanButtons(Qt::MouseButtons buttons);

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
    void updateAnglaph();

    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
};

#endif // IMAGEWIDGET_H
