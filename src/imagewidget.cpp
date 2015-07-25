#include "imagewidget.h"
#include "renderers.h"
#include <QResizeEvent>
#include <QPainter>
#include <QElapsedTimer>

ImageWidget::ImageWidget(QWidget *parent) : QWidget(parent), hBar(Qt::Horizontal, this), vBar(Qt::Vertical, this), zoom(0.0),
    swapLR(false), scrollbarsVisible(true), continuousPan(true), smoothTransform(false), anamorphicDualview(false),
    panButtons(Qt::LeftButton | Qt::MidButton), mouseTimer(this), mode(AnglaphFull), maskInterlacedHorizontal(":/masks/interlacedH.pbm"),
    maskInterlacedVertical(":/masks/interlacedV.pbm"), maskCheckerboard(":/masks/checkerboard.pbm") {

    setMouseTracking(true);
    recalculateScrollMax();

    /* Repaint whenever the user pans. */
    connect(&hBar, SIGNAL(valueChanged(int)), SLOT(update()));
    connect(&vBar, SIGNAL(valueChanged(int)), SLOT(update()));

    mouseTimer.setSingleShot(true);
    connect(&mouseTimer, &QTimer::timeout, this, &ImageWidget::hideCursor);
}

void ImageWidget::resizeEvent(QResizeEvent *e){
    /* Resize the scrollbars to be on the bottom & right and not overlap. */
    hBar.resize(e->size().width() - vBar.sizeHint().width(), hBar.sizeHint().height());
    hBar.move(0, e->size().height() - hBar.sizeHint().height());

    vBar.resize(vBar.sizeHint().width(), e->size().height() - hBar.sizeHint().height());
    vBar.move(e->size().width() - vBar.sizeHint().width(), 0);

    /* The window resize will change how the image fits in it. */
    recalculateScrollMax();
}

void ImageWidget::paintEvent(QPaintEvent *e){
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, smoothTransform);
    painter.setBrush(QBrush(Qt::black));
    painter.drawRect(e->rect());

    if(imgL.isNull() || imgR.isNull()){
        /* If either image is null tell the user. */
        painter.setPen(Qt::gray);
        QFont font;
        font.setPointSize(32);
        painter.setFont(font);
        painter.drawText(rect(), Qt::AlignCenter | Qt::TextWordWrap, tr("No Image Loaded"));
    }else if(parentWidget() && !parentWidget()->isFullScreen() &&
             (mode == InterlacedHorizontal || mode == InterlacedVertical || mode == Checkerboard)){
        /* These renderers only work properly in fullscreen mode. */
        painter.setPen(Qt::gray);
        QFont font;
        font.setPointSize(24);
        painter.setFont(font);
        painter.drawText(rect(), Qt::AlignCenter | Qt::TextWordWrap,
                         tr("%1 Display Must Be Fullscreen").arg(mode == Checkerboard ? "Checkerboard" : "Interlaced"));

        /* Keep scrollbars hidden. */
        zoom = 0.0f;
        recalculateScrollMax();
    }else{
        /* Keep track of how long it took to draw. */
        QElapsedTimer time;
        time.start();

        const QPixmap &L = swapLR ? pixmapR : pixmapL;
        const QPixmap &R = swapLR ? pixmapL : pixmapR;

        switch(mode){
        case AnglaphFull:
        case AnglaphHalf:
        case AnglaphGreyscale:
            drawSingle(anglaph, -hBar.value(), -vBar.value(), painter, zoom);
            break;
        case SidebySide:
            drawSideBySide(L, R, -hBar.value(), -vBar.value(), painter, anamorphicDualview, zoom);
            break;
        case SidebySideMLeft:
            drawSideBySide(L, R, -hBar.value(), -vBar.value(), painter, anamorphicDualview, zoom, true);
            break;
        case SidebySideMRight:
            drawSideBySide(L, R, -hBar.value(), -vBar.value(), painter, anamorphicDualview, zoom, false, true);
            break;
        case SidebySideMBoth:
            drawSideBySide(L, R, -hBar.value(), -vBar.value(), painter, anamorphicDualview, zoom, true, true);
            break;
        case TopBottom:
            drawTopBottom(L, R, -hBar.value(), -vBar.value(), painter, anamorphicDualview, zoom);
            break;
        case TopBottomMTop:
            drawTopBottom(L, R, -hBar.value(), -vBar.value(), painter, anamorphicDualview, zoom, true);
            break;
        case TopBottomMBottom:
            drawTopBottom(L, R, -hBar.value(), -vBar.value(), painter, anamorphicDualview, zoom, false, true);
            break;
        case TopBottomMBoth:
            drawTopBottom(L, R, -hBar.value(), -vBar.value(), painter, anamorphicDualview, zoom, true, true);
            break;
        case InterlacedHorizontal:
            drawInterlaced(L, R, -hBar.value(), -vBar.value(), painter, maskInterlacedHorizontal, zoom);
            break;
        case InterlacedVertical:
            drawInterlaced(L, R, -hBar.value(), -vBar.value(), painter, maskInterlacedVertical, zoom);
            break;
        case Checkerboard:
            drawInterlaced(L, R, -hBar.value(), -vBar.value(), painter, maskCheckerboard, zoom);
            break;
        case MonoLeft:
            drawSingle(L, -hBar.value(), -vBar.value(), painter, zoom);
            break;
        case MonoRight:
            drawSingle(R, -hBar.value(), -vBar.value(), painter, zoom);
            break;
        }

        qDebug("Draw time: %fms", time.nsecsElapsed() * 1.0e-6);
    }
}

bool ImageWidget::loadStereoImage(const QString &filename){
    QImage img(filename);

    imgR = img.copy(0,               0, img.width() / 2, img.height());
    imgL = img.copy(img.width() / 2, 0, img.width() / 2, img.height());
    updateImages();

    return !img.isNull();
}

void ImageWidget::updateImages(){
    pixmapL = QPixmap::fromImage(imgL);
    pixmapR = QPixmap::fromImage(imgR);
    recalculateScrollMax();
    updateAnglaph();
    repaint();
}

void ImageWidget::mouseMoveEvent(QMouseEvent *e){
    if((e->buttons() & panButtons) && (vBar.maximum() > 0 || hBar.maximum() > 0)){
        vBar.setValue(vBar.value() + lastMousePos.y() - e->y());
        hBar.setValue(hBar.value() + lastMousePos.x() - e->x());

        if(continuousPan){
            QPoint warpto = e->pos();

            /* If we're at the last pixel on either side of the screen, warp to the other side of the screen. */
            if(e->x() <= 0)
                warpto.setX(width() - 3);
            else if(e->x() >= width() - 1)
                warpto.setX(2);

            /* If we're at the last pixel on the top or bottom of the screen, warp to the other side of the screen. */
            if(e->y() <= 0)
                warpto.setY(height() - 3);
            else if(e->y() >= height() - 1)
                warpto.setY(2);

            if(warpto != e->pos())
                QCursor::setPos(mapToGlobal(warpto));

            lastMousePos = warpto;
        }else{
            lastMousePos = e->pos();
        }

        setCursor(Qt::SizeAllCursor);
    }else{
        setCursor(Qt::ArrowCursor);

        lastMousePos = e->pos();
    }

    /* Hide the cursor in 4 seconds. (unless another event happens) */
    mouseTimer.start(4000);
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *e){
    /* Any time a button used for panning is released reset to the arrow cursor. */
    if(panButtons.testFlag(e->button()))
        setCursor(Qt::ArrowCursor);
}

void ImageWidget::wheelEvent(QWheelEvent *e){
    addZoom(e->delta() * 1.0e-3);
}

void ImageWidget::enterEvent(QEvent *e){
    /* Hide the cursor in 4 seconds. (unless another event happens) */
    mouseTimer.start(4000);
}

void ImageWidget::leaveEvent(QEvent *e){
    /* We don't want the cursor to be hidden if it's outside the window. */
    mouseTimer.stop();
    setCursor(Qt::ArrowCursor);
}

void ImageWidget::recalculateScrollMax(){
    int isSidebySide = (mode == SidebySide || mode == SidebySideMLeft || mode == SidebySideMRight || mode == SidebySideMBoth) && !anamorphicDualview;
    int isTopBottom  = (mode == TopBottom  || mode == TopBottomMTop   || mode == TopBottomMBottom || mode == TopBottomMBoth) && !anamorphicDualview;

    int hmax = qMax(int((imgL.width() << isSidebySide) * zoom - width()) >> (isSidebySide + 1), 0);
    hBar.setRange(-hmax, hmax);

    int vmax = qMax(int((imgL.height() << isTopBottom) * zoom - height()) >> (isTopBottom + 1), 0);
    vBar.setRange(-vmax, vmax);

    hBar.setVisible(scrollbarsVisible && hmax != 0);
    vBar.setVisible(scrollbarsVisible && vmax != 0);
}

void ImageWidget::setZoom(qreal val){
    zoom = val;

    /* When the zoom is set we need to repaint and calculate the scrollbars' limits again. */
    recalculateScrollMax();
    repaint();
}

void ImageWidget::zoomIn(){
    addZoom( 0.1);
}

void ImageWidget::zoomOut(){
    addZoom(-0.1);
}

void ImageWidget::addZoom(qreal amount){
    /* If zoom is auto we need to figure out what that is. */
    if(zoom <= 0.0){
        int isSidebySide = (mode == SidebySide || mode == SidebySideMLeft || mode == SidebySideMRight || mode == SidebySideMBoth) && !anamorphicDualview;
        int isTopBottom  = (mode == TopBottom  || mode == TopBottomMTop   || mode == TopBottomMBottom || mode == TopBottomMBoth) && !anamorphicDualview;

        zoom = qMin(qreal(width()) / qreal(imgL.width() << isSidebySide), qreal(height()) / qreal(imgL.height() << isTopBottom));
    }

    qreal zoomorig = zoom;

    /* This way zoom changes faster when further out. */
    zoom += amount * zoom;

    zoom = qBound(0.2, zoom, 4.0);

    /* The scrollbars need adjusting whenever the zoom changes. */
    recalculateScrollMax();
    vBar.setValue(vBar.value() * zoom / zoomorig);
    hBar.setValue(hBar.value() * zoom / zoomorig);
    repaint();
}

void ImageWidget::enableSwapLR(bool enable){
    swapLR = enable;
    updateAnglaph();
    repaint();
}

void ImageWidget::showScrollbars(bool show){
    scrollbarsVisible = show;
    recalculateScrollMax();
}

void ImageWidget::enableContinuousPan(bool enable){
    continuousPan = enable;
}

void ImageWidget::enableSmoothTransform(bool enable){
    smoothTransform = enable;
    repaint();
}

void ImageWidget::enableAnamorphicDualview(bool enable){
    anamorphicDualview = enable;
    recalculateScrollMax();
    repaint();
}

void ImageWidget::hideCursor(){
    setCursor(Qt::BlankCursor);
}

void ImageWidget::setRenderMode(DrawMode m){
    mode = m;
    recalculateScrollMax();
    updateAnglaph();
    repaint();
}

void ImageWidget::setPanButtons(Qt::MouseButtons buttons){
    panButtons = buttons;
}

void ImageWidget::updateAnglaph(){
    /* Generate an anglaph pixmap to use with drawSingle() so we don't have to redo it every time you pan and such. */
    const QImage &L = swapLR ? imgR : imgL;
    const QImage &R = swapLR ? imgL : imgR;

    switch(mode){
    case AnglaphFull:
        anglaph = QPixmap::fromImage(drawAnglaph(L, R));
        break;
    case AnglaphHalf:
        anglaph = QPixmap::fromImage(drawAnglaphHalf(L, R));
        break;
    case AnglaphGreyscale:
        anglaph = QPixmap::fromImage(drawAnglaphGrey(L, R));
        break;
    default:
        /* If we aren't in anglaph mode just ignore. */
        break;
    }
}

/* Name -> Enum list. */
QMap<QString, ImageWidget::DrawMode> initDrawModeList(){
    QMap<QString, ImageWidget::DrawMode> list;
    list.insert("Anglaph, Full Color",          ImageWidget::AnglaphFull);
    list.insert("Anglaph, Half Color",          ImageWidget::AnglaphHalf);
    list.insert("Anglaph, Greyscale",           ImageWidget::AnglaphGreyscale);
    list.insert("Side by Side, No Mirror",      ImageWidget::SidebySide);
    list.insert("Side by Side, Mirror Left",    ImageWidget::SidebySideMLeft);
    list.insert("Side by Side, Mirror Right",   ImageWidget::SidebySideMRight);
    list.insert("Side by Side, Mirror Both",    ImageWidget::SidebySideMBoth);
    list.insert("Top/Bottom, No Mirror",        ImageWidget::TopBottom);
    list.insert("Top/Bottom, Mirror Top",       ImageWidget::TopBottomMTop);
    list.insert("Top/Bottom, Mirror Bottom",    ImageWidget::TopBottomMBottom);
    list.insert("Top/Bottom, Mirror Both",      ImageWidget::TopBottomMBoth);
    list.insert("Interlaced, Horizontal",       ImageWidget::InterlacedHorizontal);
    list.insert("Interlaced, Vertical",         ImageWidget::InterlacedVertical);
    list.insert("Checkerboard",                 ImageWidget::Checkerboard);
    list.insert("Mono, Left",                   ImageWidget::MonoLeft);
    list.insert("Mono, Right",                  ImageWidget::MonoRight);
    return list;
}

const QMap<QString, ImageWidget::DrawMode> ImageWidget::drawModeNames = initDrawModeList();
