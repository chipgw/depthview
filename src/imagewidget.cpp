#include "imagewidget.h"
#include "renderers.h"
#include <QResizeEvent>
#include <QPainter>
#include <QTime>

ImageWidget::ImageWidget(QWidget *parent) : QWidget(parent), mode(AnglaphFull), zoom(0.0), swapLR(false), panButtons(Qt::LeftButton | Qt::MidButton),
    mouseTimer(this), hBar(Qt::Horizontal, this), vBar(Qt::Vertical, this), continuousPan(true), scrollbarsVisible(true),
    maskInterlacedHorizontal(":/masks/interlacedH.pbm"), maskInterlacedVertical(":/masks/interlacedV.pbm"), maskCheckerboard(":/masks/checkerboard.pbm") {

    setMouseTracking(true);
    recalculatescroolmax();

    connect(&hBar, SIGNAL(valueChanged(int)), this, SLOT(update()));
    connect(&vBar, SIGNAL(valueChanged(int)), this, SLOT(update()));

    mouseTimer.setSingleShot(true);
    connect(&mouseTimer, SIGNAL(timeout()), this, SLOT(hideCursor()));
}

void ImageWidget::resizeEvent(QResizeEvent *e){
    hBar.resize(e->size().width() - vBar.sizeHint().width(), hBar.sizeHint().height());
    hBar.move(0, e->size().height() - hBar.sizeHint().height());

    vBar.resize(vBar.sizeHint().width(), e->size().height() - hBar.sizeHint().height());
    vBar.move(e->size().width() - vBar.sizeHint().width(), 0);

    recalculatescroolmax();
}

void ImageWidget::paintEvent(QPaintEvent *e){
    QPainter painter(this);
    painter.setBrush(QBrush(Qt::black));
    painter.drawRect(e->rect());

    if(imgL.isNull() && imgR.isNull()){
        painter.setPen(Qt::gray);
        QFont font;
        font.setPointSize(32);
        painter.setFont(font);
        painter.drawText(rect(), Qt::AlignCenter | Qt::TextWordWrap, tr("No Image Loaded"));
    }else{
        QTime starttime = QTime::currentTime();

        if(swapLR){
            draw(imgR, imgL, painter);
        }else{
            draw(imgL, imgR, painter);
        }

        qDebug("Draw time: %ims", starttime.msecsTo(QTime::currentTime()));
    }
}

bool ImageWidget::loadStereoImage(const QString &filename){
    QImage img(filename);

    imgR = img.copy(0,               0, img.width() / 2, img.height());
    imgL = img.copy(img.width() / 2, 0, img.width() / 2, img.height());
    recalculatescroolmax();
    updateAnglaph();
    repaint();

    return !img.isNull();
}

void ImageWidget::mouseMoveEvent(QMouseEvent *e){
    if((e->buttons() & panButtons) && (vBar.maximum() > 0 || hBar.maximum() > 0)){
        vBar.setValue(vBar.value() + lastmousepos.y() - e->y());
        hBar.setValue(hBar.value() + lastmousepos.x() - e->x());

        if(continuousPan){
            QPoint warpto = e->pos();

            if(e->x() <= 0){
                warpto.setX(width() - 3);
            }else if(e->x() >= width() - 1){
                warpto.setX(2);
            }
            if(e->y() <= 0){
                warpto.setY(height() - 3);
            }else if(e->y() >= height() - 1){
                warpto.setY(2);
            }

            if(warpto != e->pos()){
                QCursor::setPos(mapToGlobal(warpto));
            }

            lastmousepos = warpto;
        }else{
            lastmousepos = e->pos();
        }

        setCursor(Qt::SizeAllCursor);
    }else{
        setCursor(Qt::ArrowCursor);

        lastmousepos = e->pos();
    }
    mouseTimer.start(4000);
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *e){
    if(panButtons.testFlag(e->button())){
        setCursor(Qt::ArrowCursor);
    }
}

void ImageWidget::wheelEvent(QWheelEvent *e){
    addZoom(e->delta() / 1000.0);
}

void ImageWidget::recalculatescroolmax(){
    bool isSidebySide = (mode == SidebySide || mode == SidebySideMLeft || mode == SidebySideMRight || mode == SidebySideMBoth);
    bool isTopBottom  = (mode == TopBottom  || mode == TopBottomMTop   || mode == TopBottomMBottom || mode == TopBottomMBoth);

    int hmax = qMax(int((imgL.width() << isSidebySide) * zoom - width()) >> (isSidebySide + 1), 0);
    hBar.setRange(-hmax, hmax);

    int vmax = qMax(int((imgL.height() << isTopBottom) * zoom - height()) >> (isTopBottom + 1), 0);
    vBar.setRange(-vmax, vmax);

    if(scrollbarsVisible){
        hBar.setVisible(hmax != 0);
        vBar.setVisible(vmax != 0);
    }else{
        hBar.setVisible(false);
        vBar.setVisible(false);
    }
}

void ImageWidget::setZoom(qreal val){
    zoom = val;
    recalculatescroolmax();
    repaint();
}

void ImageWidget::zoomIn(){
    addZoom( 0.1);
}

void ImageWidget::zoomOut(){
    addZoom(-0.1);
}

void ImageWidget::addZoom(qreal amount){
    if(zoom <= 0.0){
        if(mode == SidebySide || mode == SidebySideMLeft || mode == SidebySideMRight || mode == SidebySideMBoth){
            zoom = qMin(qreal(width()) / (imgL.width() * 2.0), qreal(height()) / qreal(imgL.height()));
        }else if(mode == TopBottom || mode == TopBottomMTop || mode == TopBottomMBottom || mode == TopBottomMBoth){
            zoom = qMin(qreal(width()) / qreal(imgL.width()), qreal(height()) / (imgL.height() * 2.0));
        }else{
            zoom = qMin(qreal(width()) / qreal(imgL.width()), qreal(height()) / qreal(imgL.height()));
        }
    }
    qreal zoomorig = zoom;
    zoom += amount * zoom;
    zoom = qBound(0.2, zoom, 4.0);
    recalculatescroolmax();
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
    recalculatescroolmax();
}

void ImageWidget::enableContinuousPan(bool enable){
    continuousPan = enable;
}

void ImageWidget::hideCursor(){
    setCursor(Qt::BlankCursor);
}

void ImageWidget::draw(const QImage &L, const QImage &R, QPainter &painter){
    switch(mode){
    case AnglaphFull:
    case AnglaphHalf:
    case AnglaphGreyscale:
        drawSingle(anglaph, -hBar.value(), -vBar.value(), painter, zoom);
        break;
    case SidebySide:
        drawSideBySide(L, R, -hBar.value(), -vBar.value(), painter, zoom);
        break;
    case SidebySideMLeft:
        drawSideBySide(L, R, -hBar.value(), -vBar.value(), painter, zoom, true);
        break;
    case SidebySideMRight:
        drawSideBySide(L, R, -hBar.value(), -vBar.value(), painter, zoom, false, true);
        break;
    case SidebySideMBoth:
        drawSideBySide(L, R, -hBar.value(), -vBar.value(), painter, zoom, true, true);
        break;
    case TopBottom:
        drawTopBottom(L, R, -hBar.value(), -vBar.value(), painter, zoom);
        break;
    case TopBottomMTop:
        drawTopBottom(L, R, -hBar.value(), -vBar.value(), painter, zoom, true);
        break;
    case TopBottomMBottom:
        drawTopBottom(L, R, -hBar.value(), -vBar.value(), painter, zoom, false, true);
        break;
    case TopBottomMBoth:
        drawTopBottom(L, R, -hBar.value(), -vBar.value(), painter, zoom, true, true);
        break;
    case InterlacedHorizontal:
        drawInterlaced(L, R, -hBar.value(), -vBar.value(), painter, maskInterlacedHorizontal, zoom, this);
        break;
    case InterlacedVertical:
        drawInterlaced(L, R, -hBar.value(), -vBar.value(), painter, maskInterlacedVertical, zoom, this);
        break;
    case Checkerboard:
        drawInterlaced(L, R, -hBar.value(), -vBar.value(), painter, maskCheckerboard, zoom, this);
        break;
    case MonoLeft:
        drawSingle(L, -hBar.value(), -vBar.value(), painter, zoom);
        break;
    case MonoRight:
        drawSingle(R, -hBar.value(), -vBar.value(), painter, zoom);
        break;
    }
}

void ImageWidget::setRenderMode(DrawMode m){
    mode = m;
    recalculatescroolmax();
    updateAnglaph();
    repaint();
}

void ImageWidget::setPanButtons(Qt::MouseButtons buttons){
    panButtons = buttons;
}

void ImageWidget::updateAnglaph(){
    const QImage &L = swapLR ? imgL : imgR;
    const QImage &R = swapLR ? imgR : imgL;
    switch(mode){
    case AnglaphFull:
        anglaph = drawAnglaph(L, R);
        break;
    case AnglaphHalf:
        anglaph = drawAnglaphHalf(L, R);
        break;
    case AnglaphGreyscale:
        anglaph = drawAnglaphGrey(L, R);
        break;
    default: break;
    }
}

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
