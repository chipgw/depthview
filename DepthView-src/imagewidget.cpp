#include "imagewidget.h"
#include <QResizeEvent>
#include <QPainter>

ImageWidget::ImageWidget(QWidget *parent) : QWidget(parent){
    this->hBar = new QScrollBar(Qt::Horizontal, this);
    this->vBar = new QScrollBar(Qt::Vertical, this);
    //this->hBar->hide();
    //this->vBar->hide();
    this->hBar->setMinimum(-100);
    this->vBar->setMinimum(-100);
    this->setMouseTracking(true);
    zoom = 1;
    smooth = true;
    this->renderer = new StereoRender();
    connect(this->hBar, SIGNAL(valueChanged(int)),this,SLOT(repaint()));
    connect(this->vBar, SIGNAL(valueChanged(int)),this,SLOT(repaint()));
}

void ImageWidget::resizeEvent(QResizeEvent *e){
    this->hBar->resize(e->size().width()-20, 20);
    this->hBar->move(0, e->size().height()-20);
    int hmax = (imgL.width() - this->width())/2;
    this->hBar->setMaximum( hmax);
    this->hBar->setMinimum(-hmax);

    this->vBar->resize(20, e->size().height()-20);
    this->vBar->move(e->size().width()-20,0);
    int vmax = (imgL.height() - this->height())/2;
    this->vBar->setMaximum( vmax);
    this->vBar->setMinimum(-vmax);
}

void ImageWidget::paintEvent(QPaintEvent *e){
    QPainter painter(this);
    painter.drawImage(0,0,renderer->draw(imgL,imgR,-hBar->value(),-vBar->value(),this->width(),this->height(), this->zoom));
}
void ImageWidget::loadStereoImage(QString filename){
    QImage img(filename);
    this->imgL = img.copy(0,0,img.width()/2,img.height());
    this->imgR = img.copy(img.width()/2,0,img.width()/2,img.height());
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
