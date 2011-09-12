#include "depthviewwindow.h"
#include "ui_depthviewwindow.h"
#include <QKeyEvent>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

DepthViewWindow::DepthViewWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::DepthViewWindow){
    ui->setupUi(this);
    fileFilters << "*.jps" << "*.pns";
    ui->imageWidget->addActions(ui->menubar->actions());
    ui->imageWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
}

DepthViewWindow::~DepthViewWindow(){
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
    QString filename = QFileDialog::getOpenFileName(this,"Open Image", "", "Stereo Image Files (*.jps *.pns)");
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
    StereoRender::colormult = 1;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionHalf_Color_triggered(){
    StereoRender::colormult = 0.5;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionGreyscale_triggered(){
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

}
