#include "imagewidget.h"
#include "renderers.h"
#include <QResizeEvent>
#include <QPainter>
#include <QTime>
#include <QDebug>
#include <QScrollArea>

ImageWidget::ImageWidget(QWidget *parent) : QWidget(parent), mode(AnglaphFull), zoom(0.0f), swapLR(false),
    mouseTimer(this), hBar(Qt::Horizontal, this), vBar(Qt::Vertical, this), enableContinuousPan(true), showScrollbars(true) {

    this->setMouseTracking(true);
    this->recalculatescroolmax();

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

    this->recalculatescroolmax();
}

void ImageWidget::paintEvent(QPaintEvent *e){
    QPainter painter(this);
    if(imgL.isNull() && imgR.isNull()){
        painter.setBrush(QBrush(Qt::black));
        painter.drawRect(e->rect());

        painter.setPen(Qt::gray);
        QFont font;
        font.setPointSize(32);
        painter.setFont(font);
        painter.drawText(rect(), Qt::AlignCenter | Qt::TextWordWrap, tr("No Image Loaded"));
    }else{
        QTime starttime = QTime::currentTime();

        if(swapLR){
            painter.drawImage(0 ,0, draw(imgR, imgL));
        }else{
            painter.drawImage(0, 0, draw(imgL, imgR));
        }

        qDebug() << "Draw time:" << starttime.msecsTo(QTime::currentTime()) << "ms";
    }
}

bool ImageWidget::loadStereoImage(const QString &filename){
    QImage img(filename);

    this->imgL = img.copy(0,               0, img.width() / 2, img.height());
    this->imgR = img.copy(img.width() / 2, 0, img.width() / 2, img.height());
    this->recalculatescroolmax();

    return !img.isNull();
}

void ImageWidget::mouseMoveEvent(QMouseEvent *e){
    if(e->buttons().testFlag(Qt::MiddleButton)){
        vBar.setValue(vBar.value() + lastmousepos.y() - e->y());
        hBar.setValue(hBar.value() + lastmousepos.x() - e->x());

        if(enableContinuousPan){
            QPoint warpto = e->pos();

            if(e->x() <= 0){
                warpto.setX(this->width() - 3);
            }else if(e->x() >= this->width() - 1){
                warpto.setX(2);
            }
            if(e->y() <= 0){
                warpto.setY(this->height() - 3);
            }else if(e->y() >= this->height() - 1){
                warpto.setY(2);
            }

            if(warpto != e->pos()){
                QCursor::setPos(this->mapToGlobal(warpto));
            }

            this->lastmousepos = warpto;
        }else{
            this->lastmousepos = e->pos();
        }

        this->setCursor(Qt::SizeAllCursor);
    }else{
        this->setCursor(Qt::ArrowCursor);

        this->lastmousepos = e->pos();
    }
    mouseTimer.start(4000);
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
    int height = imgL.height();
    if(mode == SidebySide || mode == SidebySideMLeft || mode == SidebySideMRight || mode == SidebySideMBoth){
        width *= 2;
    }else if(mode == TopBottom || mode == TopBottomMTop || mode == TopBottomMBottom || mode == TopBottomMBoth){
        height *= 2;
    }
    int hmax = qMax(int(width  * zoom - this->width())  / 2, 0);
    hBar.setRange(-hmax, hmax);

    int vmax = qMax(int(height * zoom - this->height()) / 2, 0);
    vBar.setRange(-vmax, vmax);

    if(showScrollbars){
        hBar.setVisible(hmax != 0);
        vBar.setVisible(vmax != 0);
    }else{
        hBar.setVisible(false);
        vBar.setVisible(false);
    }
}

void ImageWidget::setZoom(float val){
    zoom = val;
    this->recalculatescroolmax();
    this->repaint();
}

void ImageWidget::zoomIn(){
    addZoom( 0.1f);
}

void ImageWidget::zoomOut(){
    addZoom(-0.1f);
}

void ImageWidget::addZoom(float amount){
    if(zoom <= 0.0f){
        if(mode == SidebySide || mode == SidebySideMLeft || mode == SidebySideMRight || mode == SidebySideMBoth){
            zoom = qMin((float)this->width() / (imgL.width() * 2.0f), (float)this->height() / (float)imgL.height());
        }else if(mode == TopBottom || mode == TopBottomMTop || mode == TopBottomMBottom || mode == TopBottomMBoth){
            zoom = qMin((float)this->width() / imgL.width(), (float)this->height() / ((float)imgL.height() * 2.0f));
        }else{
            zoom = qMin((float)this->width() / (float)imgL.width(), (float)this->height() / (float)imgL.height());
        }
    }
    float zoomorig = zoom;
    zoom += amount * zoom;
    zoom = qBound(0.2f, zoom, 4.0f);
    this->recalculatescroolmax();
    vBar.setValue(vBar.value() * zoom / zoomorig);
    hBar.setValue(hBar.value() * zoom / zoomorig);
    this->repaint();
}

void ImageWidget::hideCursor(){
    this->setCursor(Qt::BlankCursor);
}

QImage ImageWidget::draw(const QImage &L, const QImage &R){
    switch(mode){
    case AnglaphFull:
        return drawAnglaph(L, R, -hBar.value(), -vBar.value(), this->width(), this->height(), this->zoom, 1.0f);
        break;
    case AnglaphHalf:
        return drawAnglaph(L, R, -hBar.value(), -vBar.value(), this->width(), this->height(), this->zoom, 0.5f);
        break;
    case AnglaphGreyscale:
        return drawAnglaph(L, R, -hBar.value(), -vBar.value(), this->width(), this->height(), this->zoom, 0.0f);
        break;
    case SidebySide:
        return drawSideBySide(L, R, -hBar.value(), -vBar.value(), this->width(), this->height(), this->zoom);
        break;
    case SidebySideMLeft:
        return drawSideBySide(L, R, -hBar.value(), -vBar.value(), this->width(), this->height(), this->zoom, true);
        break;
    case SidebySideMRight:
        return drawSideBySide(L, R, -hBar.value(), -vBar.value(), this->width(), this->height(), this->zoom, false, true);
        break;
    case SidebySideMBoth:
        return drawSideBySide(L, R, -hBar.value(), -vBar.value(), this->width(), this->height(), this->zoom, true, true);
        break;
    case TopBottom:
        return drawTopBottom(L, R, -hBar.value(), -vBar.value(), this->width(), this->height(), this->zoom);
        break;
    case TopBottomMTop:
        return drawTopBottom(L, R, -hBar.value(), -vBar.value(), this->width(), this->height(), this->zoom, true);
        break;
    case TopBottomMBottom:
        return drawTopBottom(L, R, -hBar.value(), -vBar.value(), this->width(), this->height(), this->zoom, false, true);
        break;
    case TopBottomMBoth:
        return drawTopBottom(L, R, -hBar.value(), -vBar.value(), this->width(), this->height(), this->zoom, true, true);
        break;
    case InterlacedHorizontal:
        return drawInterlaced(L, R, -hBar.value(), -vBar.value(), this->width(), this->height(), this->zoom, true, this);
        break;
    case InterlacedVertical:
        return drawInterlaced(L, R, -hBar.value(), -vBar.value(), this->width(), this->height(), this->zoom, false, this);
        break;
    case Checkerboard:
        return drawCheckerboard(L, R, -hBar.value(), -vBar.value(), this->width(), this->height(), this->zoom, this);
        break;
    case MonoLeft:
        return drawSingle(L, -hBar.value(), -vBar.value(), this->width(), this->height(), this->zoom);
        break;
    case MonoRight:
        return drawSingle(R, -hBar.value(), -vBar.value(), this->width(), this->height(), this->zoom);
        break;
    }
    return QImage();
}

void ImageWidget::mouseDoubleClickEvent(QMouseEvent *e){
    if(e->button() == Qt::LeftButton){
        emit doubleClicked();
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
