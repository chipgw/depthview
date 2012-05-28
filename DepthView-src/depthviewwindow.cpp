#include "depthviewwindow.h"
#include "ui_depthviewwindow.h"
#include <QKeyEvent>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <settingswindow.h>

DepthViewWindow::DepthViewWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::DepthViewWindow){
    ui->setupUi(this);
    fileFilters << "*.jps" << "*.pns";
    ui->imageWidget->addActions(ui->menubar->actions());
    ui->imageWidget->setContextMenuPolicy(Qt::ActionsContextMenu);

    this->loadSettings();
}

DepthViewWindow::~DepthViewWindow(){
    settings.setValue("lastrun", QDate::currentDate().toString());
    delete ui;
}

bool DepthViewWindow::loadImage(QString filename){
    QFileInfo info(filename);
    if(info.exists() && (info.suffix() == "jps" || info.suffix() == "pns")){
        QDir::setCurrent(info.path());
        currentFile = info.fileName();
        ui->imageWidget->loadStereoImage(currentFile);
        this->setWindowTitle(currentFile);
        ui->imageWidget->repaint();
        return true;
    }
    else{
        return false;
    }
}
bool DepthViewWindow::showLoadImageDialog(){
    QString filename = QFileDialog::getOpenFileName(this,tr("Open Image"), "", tr("Stereo Image Files (*.jps *.pns)"));
    return loadImage(filename);
}

void DepthViewWindow::on_actionAbout_Qt_triggered(){
    QMessageBox::aboutQt(this);
}

void DepthViewWindow::on_actionExit_triggered(){
    this->close();
}

void DepthViewWindow::on_actionOpen_triggered(){
    this->showLoadImageDialog();
}

void DepthViewWindow::on_actionFull_Color_triggered(){
    delete ui->imageWidget->renderer;
    ui->imageWidget->renderer = new StereoRender();
    StereoRender::colormult = 1;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionHalf_Color_triggered(){
    delete ui->imageWidget->renderer;
    ui->imageWidget->renderer = new StereoRender();
    StereoRender::colormult = 0.5;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionGreyscale_triggered(){
    delete ui->imageWidget->renderer;
    ui->imageWidget->renderer = new StereoRender();
    StereoRender::colormult = 0;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionShow_MenuBar_toggled(bool val){
    ui->menubar->setVisible(val);
}

void DepthViewWindow::on_actionFullscreen_toggled(bool val){
    if(val){
        setWindowState(windowState() | Qt::WindowFullScreen);
        ui->actionShow_MenuBar->setChecked(false);
    }
    else{
        setWindowState(windowState() & ~Qt::WindowFullScreen);
        ui->actionShow_MenuBar->setChecked(true);
    }
}

void DepthViewWindow::on_actionNext_triggered(){
    QStringList entryList = QDir::current().entryList(fileFilters);
    int index = entryList.indexOf(currentFile)+1;
    if(index < 0){
        index = entryList.count()-1;
    }
    else if(index >= entryList.count()){
        index = 0;
    }
    loadImage(entryList[index]);
    return;
}

void DepthViewWindow::on_actionPrevious_triggered(){
    QStringList entryList = QDir::current().entryList(fileFilters);
    int index = entryList.indexOf(currentFile)-1;
    if(index < 0){
        index = entryList.count()-1;
    }
    else if(index >= entryList.count()){
        index = 0;
    }
    loadImage(entryList[index]);
    return;
}

void DepthViewWindow::mousePressEvent(QMouseEvent *e){
    if(e->buttons() == Qt::XButton2)
        this->on_actionNext_triggered();
    if(e->buttons() == Qt::XButton1)
        this->on_actionPrevious_triggered();
}
void DepthViewWindow::mouseDoubleClickEvent(QMouseEvent *e){
    if(e->button() == Qt::LeftButton)
        ui->actionFullscreen->toggle();
}

void DepthViewWindow::on_actionSave_As_triggered(){
    QString filename = QFileDialog::getSaveFileName(this,tr("Save Image"), "", tr("Stereo Image Files (*.jps *.pns);;Image Files (*.bmp *.jpg *.jpeg *.png *.ppm *.tiff *.xbm *.xpm)"));
    QImage out;
    if(filename.contains(".jps") || filename.contains(".pns")){
        SideBySideRender renderer;
        bool tempmirrorL = SideBySideRender::mirrorL;
        bool tempmirrorR = SideBySideRender::mirrorR;
        SideBySideRender::mirrorL = false;
        SideBySideRender::mirrorR = false;
        out = renderer.draw(ui->imageWidget->imgL,ui->imageWidget->imgR,0,0);
        SideBySideRender::mirrorL = tempmirrorL;
        SideBySideRender::mirrorR = tempmirrorR;
        if(filename.contains(".jps")){
            if(!out.isNull()){
                out.save(filename, "JPG");
            }
        }
        if(filename.contains(".pns")){
            if(!out.isNull()){
                out.save(filename, "PNG");
            }
        }
    }
    else{
        out = ui->imageWidget->renderer->draw(ui->imageWidget->imgL,ui->imageWidget->imgR,0,0);
        if(!out.isNull()){
            out.save(filename);
        }
    }
}

void DepthViewWindow::on_actionNo_Mirror_triggered(){
    delete ui->imageWidget->renderer;
    ui->imageWidget->renderer = new SideBySideRender();
    SideBySideRender::mirrorL = false;
    SideBySideRender::mirrorR = false;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionMirror_Left_triggered(){
    delete ui->imageWidget->renderer;
    ui->imageWidget->renderer = new SideBySideRender();
    SideBySideRender::mirrorL = true;
    SideBySideRender::mirrorR = false;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionMirror_Right_triggered(){
    delete ui->imageWidget->renderer;
    ui->imageWidget->renderer = new SideBySideRender();
    SideBySideRender::mirrorL = false;
    SideBySideRender::mirrorR = true;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionMirror_Both_triggered(){
    delete ui->imageWidget->renderer;
    ui->imageWidget->renderer = new SideBySideRender();
    SideBySideRender::mirrorL = true;
    SideBySideRender::mirrorR = true;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionFit_triggered(){
    ui->imageWidget->setZoom(0);
}

void DepthViewWindow::on_actionzoom100_triggered(){
    ui->imageWidget->setZoom(1);
}

void DepthViewWindow::on_actionzoom50_triggered(){
    ui->imageWidget->setZoom(0.5);
}

void DepthViewWindow::on_actionzoom200_triggered(){
    ui->imageWidget->setZoom(2);
}

void DepthViewWindow::on_actionHorizontal_triggered(){
    delete ui->imageWidget->renderer;
    ui->imageWidget->renderer = new InterlacedRender(this);
    InterlacedRender::horizontal = true;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionVertical_triggered(){
    delete ui->imageWidget->renderer;
    ui->imageWidget->renderer = new InterlacedRender(this);
    InterlacedRender::horizontal = false;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionOptions_triggered(){
    SettingsWindow settingsdialog(&settings, this);
    if(settingsdialog.exec() == QDialog::Accepted){
        this->loadSettings();
    }
}

void DepthViewWindow::on_actionAbout_triggered(){
    QMessageBox::about(this, "About DepthView", "DepthView is a basic application for viewing stereo 3D image files. http://sourceforge.net/projects/depthview/");
}

void DepthViewWindow::loadSettings(){
    if(settings.contains("defaultrender")){
        QString renderer = settings.value("defaultrender").toString();
        if(renderer == "Anglaph, Full Color"){
            this->on_actionFull_Color_triggered();
        }
        else if(renderer == "Anglaph, Half Color"){
            this->on_actionHalf_Color_triggered();
        }
        else if(renderer == "Anglaph, Greyscale"){
            this->on_actionGreyscale_triggered();
        }
        else if(renderer == "Side by Side, No Mirror"){
            this->on_actionNo_Mirror_triggered();
        }
        else if(renderer == "Side by Side, Mirror Left"){
            this->on_actionMirror_Left_triggered();
        }
        else if(renderer == "Side by Side, Mirror Right"){
            this->on_actionMirror_Right_triggered();
        }
        else if(renderer == "Side by Side, Mirror Both"){
            this->on_actionMirror_Both_triggered();
        }
        else if(renderer == "Interlaced, Horizontal"){
            this->on_actionHorizontal_triggered();
        }
        else if(renderer == "Interlaced, Vertical"){
            this->on_actionVertical_triggered();
        }
        else if(renderer == "Checkerboard"){
            this->on_actionCheckerboard_triggered();
        }
    }
    if(settings.contains("startfullscreen")){
        ui->actionFullscreen->setChecked(settings.value("startfullscreen").toBool());
    }
}

void DepthViewWindow::on_actionCheckerboard_triggered(){
    delete ui->imageWidget->renderer;
    ui->imageWidget->renderer = new CheckerBoardRender(this);
    ui->imageWidget->repaint();
}
