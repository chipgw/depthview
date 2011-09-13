#include "imagewidget.h"
#include <QResizeEvent>
#include <QPainter>

ImageWidget::ImageWidget(QWidget *parent) : QWidget(parent){
    this->hBar = new QScrollBar(Qt::Horizontal, this);
    this->vBar = new QScrollBar(Qt::Vertical, this);
    this->hBar->setMinimum(-100);
    this->vBar->setMinimum(-100);
    this->setMouseTracking(true);
    zoom = 1;
    smooth = true;
    this->renderer = new StereoRender();
    this->recalculatescroolmax();
    connect(this->hBar, SIGNAL(valueChanged(int)),this,SLOT(update()));
    connect(this->vBar, SIGNAL(valueChanged(int)),this,SLOT(update()));
}

void ImageWidget::resizeEvent(QResizeEvent *e){
    this->hBar->resize(e->size().width()-15, 15);
    this->hBar->move(0, e->size().height()-15);

    this->vBar->resize(15, e->size().height()-15);
    this->vBar->move(e->size().width()-15,0);

    this->recalculatescroolmax();
}

void ImageWidget::paintEvent(QPaintEvent *e){
    QPainter painter(this);
    painter.drawImage(0,0,renderer->draw(imgL,imgR,-hBar->value(),-vBar->value(),this->width(),this->height(), this->zoom));
}
void ImageWidget::loadStereoImage(QString filename){
    QImage img(filename);
    this->imgL = img.copy(0,0,img.width()/2,img.height());
    this->imgR = img.copy(img.width()/2,0,img.width()/2,img.height());
    this->recalculatescroolmax();
}

void ImageWidget::mouseMoveEvent(QMouseEvent *e){
    if(e->buttons().testFlag(Qt::LeftButton)){
        this->lastmousepos = e->pos();
        this->setCursor(Qt::ArrowCursor);
    }
    else if(e->buttons().testFlag(Qt::MiddleButton)){
        vBar->setValue(vBar->value() - (e->y() - lastmousepos.y() *1.0f)/this->height()*vBar->maximum()*2);
        hBar->setValue(hBar->value() - (e->x() - lastmousepos.x() *1.0f)/this->width()*hBar->maximum()*2);

        this->setCursor(Qt::SizeAllCursor);
        this->lastmousepos = e->pos();
    }
    else if(e->buttons().testFlag(Qt::RightButton)){
        this->setCursor(Qt::ArrowCursor);
    }
    else{
        this->setCursor(Qt::ArrowCursor);
        this->lastmousepos = e->pos();
    }
}
void ImageWidget::wheelEvent(QWheelEvent *e){
    float zoomorig = zoom;
    zoom += e->delta()/1000.0f*zoom;
    zoom = qMax(zoom, 0.2f);
    zoom = qMin(zoom, 4.0f);
    this->recalculatescroolmax();
    vBar->setValue(vBar->value()*zoom/zoomorig);
    hBar->setValue(hBar->value()*zoom/zoomorig);
    this->repaint();
}

void ImageWidget::recalculatescroolmax(){
    int hmax = qMax((imgL.width()*zoom - this->width())/2,0.0f);
    this->hBar->setMaximum( hmax);
    this->hBar->setMinimum(-hmax);

    int vmax = qMax((imgL.height()*zoom - this->height())/2,0.0f);
    this->vBar->setMaximum( vmax);
    this->vBar->setMinimum(-vmax);

    hBar->setVisible(hmax != 0);
    vBar->setVisible(vmax != 0);
}
