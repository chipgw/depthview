#include "imagewidget.h"
#include <QResizeEvent>
#include <QPainter>

ImageWidget::ImageWidget(QWidget *parent) : QWidget(parent){
    this->hBar = new QScrollBar(Qt::Horizontal, this);
    this->vBar = new QScrollBar(Qt::Vertical, this);
    this->hBar->setMinimum(-100);
    this->vBar->setMinimum(-100);
    this->setMouseTracking(true);
    zoom = 0;
    smooth = false;
    swapLR = false;
    this->recalculatescroolmax();
    connect(this->hBar, SIGNAL(valueChanged(int)),this,SLOT(update()));
    connect(this->vBar, SIGNAL(valueChanged(int)),this,SLOT(update()));

    mouseTimer = new QTimer(this);
    mouseTimer->setSingleShot(true);
    connect(mouseTimer, SIGNAL(timeout()), this, SLOT(hideCursor()));
}

void ImageWidget::resizeEvent(QResizeEvent *e){
    this->hBar->resize(e->size().width() - 15, 15);
    this->hBar->move(0, e->size().height() - 15);

    this->vBar->resize(15, e->size().height() - 15);
    this->vBar->move(e->size().width() - 15,0);

    this->recalculatescroolmax();
}

void ImageWidget::paintEvent(QPaintEvent *e){
    QPainter painter(this);
    if(swapLR){
        painter.drawImage(0 ,0, draw(imgR, imgL));
    }
    else{
        painter.drawImage(0, 0, draw(imgL, imgR));
    }
}

void ImageWidget::loadStereoImage(QString filename){
    QImage img(filename);
    this->imgL = img.copy(0,               0, img.width() / 2, img.height());
    this->imgR = img.copy(img.width() / 2, 0, img.width() / 2, img.height());
    this->recalculatescroolmax();
}

void ImageWidget::mouseMoveEvent(QMouseEvent *e){
    if(e->buttons().testFlag(Qt::MiddleButton)){
        vBar->setValue(vBar->value() + lastmousepos.y() - e->y());
        hBar->setValue(hBar->value() + lastmousepos.x() - e->x());

        this->setCursor(Qt::SizeAllCursor);
    }else{
        this->setCursor(Qt::ArrowCursor);
    }
    mouseTimer->start(4000);
    this->lastmousepos = e->pos();
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *e){
    if(e->button() == Qt::MiddleButton){
        this->setCursor(Qt::ArrowCursor);
    }
}

void ImageWidget::wheelEvent(QWheelEvent *e){
    addZoom(e->delta() / 1000.0f);
}

void ImageWidget::recalculatescroolmax(){
    int width = imgL.width();
    if(mode == SidebySide || mode == SidebySideMLeft || mode == SidebySideMRight || mode == SidebySideMBoth){
        width *= 2;
    }
    int hmax = qMax((width  * zoom - this->width())  / 2.0f, 0.0f);
    this->hBar->setMaximum( hmax);
    this->hBar->setMinimum(-hmax);

    int vmax = qMax((imgL.height() * zoom - this->height()) / 2.0f, 0.0f);
    this->vBar->setMaximum( vmax);
    this->vBar->setMinimum(-vmax);

    hBar->setVisible(hmax != 0);
    vBar->setVisible(vmax != 0);
}

void ImageWidget::setZoom(float val){
    zoom = val;
    this->recalculatescroolmax();
    this->repaint();
}

float ImageWidget::getZoom(){
    return this->zoom;
}

void ImageWidget::zoomIn(){
    addZoom( 0.1f);
}

void ImageWidget::zoomOut(){
    addZoom(-0.1f);
}

void ImageWidget::addZoom(float amount){
    if(zoom == 0){
        if(mode == SidebySide || mode == SidebySideMLeft || mode == SidebySideMRight || mode == SidebySideMBoth){
            zoom = qMin((float)this->width() / (imgL.width() * 2.0f), (float)this->height() / (float)imgL.height());
        }else{
            zoom = qMin((float)this->width() / (float)imgL.width(), (float)this->height() / (float)imgL.height());
        }
    }
    float zoomorig = zoom;
    zoom += amount * zoom;
    zoom = qMax(zoom, 0.2f);
    zoom = qMin(zoom, 4.0f);
    this->recalculatescroolmax();
    vBar->setValue(vBar->value()*zoom/zoomorig);
    hBar->setValue(hBar->value()*zoom/zoomorig);
    this->repaint();
}

void ImageWidget::hideCursor(){
    this->setCursor(Qt::BlankCursor);
}

QImage ImageWidget::draw(const QImage &L, const QImage &R){
    switch(mode){
    case AnglaphFull:
        return drawAnglaph(L, R, -hBar->value(), -vBar->value(), this->width(), this->height(), this->zoom, 1.0f);
        break;
    case AnglaphHalf:
        return drawAnglaph(L, R, -hBar->value(), -vBar->value(), this->width(), this->height(), this->zoom, 0.5f);
        break;
    case AnglaphGreyscale:
        return drawAnglaph(L, R, -hBar->value(), -vBar->value(), this->width(), this->height(), this->zoom, 0.0f);
        break;
    case SidebySide:
        return drawSideBySide(L, R, -hBar->value(), -vBar->value(), this->width(), this->height(), this->zoom);
        break;
    case SidebySideMLeft:
        return drawSideBySide(L, R, -hBar->value(), -vBar->value(), this->width(), this->height(), this->zoom, true);
        break;
    case SidebySideMRight:
        return drawSideBySide(L, R, -hBar->value(), -vBar->value(), this->width(), this->height(), this->zoom, false, true);
        break;
    case SidebySideMBoth:
        return drawSideBySide(L, R, -hBar->value(), -vBar->value(), this->width(), this->height(), this->zoom, true, true);
        break;
    case InterlacedHorizontal:
        return drawInterlaced(L, R, -hBar->value(), -vBar->value(), this->width(), this->height(), this->zoom, true, this);
        break;
    case InterlacedVertical:
        return drawInterlaced(L, R, -hBar->value(), -vBar->value(), this->width(), this->height(), this->zoom, false, this);
        break;
    case Checkerboard:
        return drawCheckerboard(L, R, -hBar->value(), -vBar->value(), this->width(), this->height(), this->zoom, this);
        break;
    case MonoLeft:
        return drawSingle(L, -hBar->value(), -vBar->value(), this->width(), this->height(), this->zoom);
        break;
    case MonoRight:
        return drawSingle(R, -hBar->value(), -vBar->value(), this->width(), this->height(), this->zoom);
        break;
    }
    return QImage();
}
