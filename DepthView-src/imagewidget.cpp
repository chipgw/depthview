#include "imagewidget.h"
#include <QResizeEvent>
#include <QPainter>

ImageWidget::ImageWidget(QWidget *parent) : QWidget(parent){
    this->hBar = new QScrollBar(Qt::Horizontal, this);
    this->vBar = new QScrollBar(Qt::Vertical, this);
    this->hBar->hide();
    this->vBar->hide();
    zoom = 1;
    smooth = true;
}

void ImageWidget::resizeEvent(QResizeEvent *e){
    this->hBar->resize(e->size().width()-20, 20);
    this->hBar->move(0, e->size().height()-20);

    this->vBar->resize(20, e->size().height()-20);
    this->vBar->move(e->size().width()-20,0);
}

void ImageWidget::paintEvent(QPaintEvent *e){
    QPainter painter(this);
    Qt::TransformationMode mode = Qt::FastTransformation;
    if (smooth){
        mode = Qt::SmoothTransformation;
    }
    painter.drawImage(0,0,renderer.draw(imgL.scaled(this->size(),Qt::KeepAspectRatio, mode),imgR.scaled(this->size(),Qt::KeepAspectRatio, mode),0,0,this->width(),this->height()));
    // imgL.scaledToWidth(imgL.width()*zoom),imgR.scaledToWidth(imgR.width()*zoom)
}
void ImageWidget::loadStereoImage(QString filename){
    QImage img(filename);
    this->imgL = img.copy(0,0,img.width()/2,img.height());
    this->imgR = img.copy(img.width()/2,0,img.width()/2,img.height());
}
