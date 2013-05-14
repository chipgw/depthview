#include "depthviewwindow.h"
#include "ui_depthviewwindow.h"
#include "exportdialog.h"
#include "settingswindow.h"
#include "version.h"
#include <QKeyEvent>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QTime>
#include <QMimeData>

DepthViewWindow::DepthViewWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::DepthViewWindow){
    ui->setupUi(this);
    fileFilters << "*.jps" << "*.pns";
    ui->imageWidget->addActions(ui->menubar->actions());
    ui->imageWidget->setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(ui->imageWidget, SIGNAL(doubleClicked()), ui->actionFullscreen, SLOT(toggle()));

    this->loadSettings();
}

DepthViewWindow::~DepthViewWindow(){
    settings.setValue("lastrun", QDate::currentDate().toString());
    delete ui;
}

bool DepthViewWindow::loadImage(QString filename){
    QFileInfo info(filename);
    if(info.exists() && (info.suffix().toLower() == "jps" || info.suffix().toLower() == "pns")){
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
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Stereo Image Files (*.jps *.pns)"));
    if(!filename.isEmpty()){
        return loadImage(filename);
    }
    return false;
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
    ui->imageWidget->mode = ImageWidget::AnglaphFull;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionHalf_Color_triggered(){
    ui->imageWidget->mode = ImageWidget::AnglaphHalf;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionGreyscale_triggered(){
    ui->imageWidget->mode = ImageWidget::AnglaphGreyscale;
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
    if(!entryList.empty()){
        int index = entryList.indexOf(currentFile) + 1;
        if(index < 0){
            index = entryList.count() - 1;
        }
        else if(index >= entryList.count()){
            index = 0;
        }
        loadImage(entryList[index]);
    }
}

void DepthViewWindow::on_actionPrevious_triggered(){
    QStringList entryList = QDir::current().entryList(fileFilters);
    if(!entryList.empty()){
        int index = entryList.indexOf(currentFile) - 1;
        if(index < 0){
            index = entryList.count() - 1;
        }
        else if(index >= entryList.count()){
            index = 0;
        }
        loadImage(entryList[index]);
    }
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

    if(filename.contains(".jps", Qt::CaseInsensitive) || filename.contains(".pns", Qt::CaseInsensitive)){
        QImage out = drawSideBySide(ui->imageWidget->imgL,ui->imageWidget->imgR, 0, 0);

        if(filename.contains(".jps", Qt::CaseInsensitive)){
            if(!out.isNull()){
                out.save(filename, "JPG");
            }
        }
        if(filename.contains(".pns", Qt::CaseInsensitive)){
            if(!out.isNull()){
                out.save(filename, "PNG");
            }
        }
    }
    else if(!filename.isNull()){
        ExportDialog dialog(this);

        if(dialog.exec() == QDialog::Accepted){
            if(dialog.anglaph){
                QImage out = drawAnglaph(ui->imageWidget->imgL, ui->imageWidget->imgR, 0, 0, 0, 0, 1.0f, dialog.colormult);

                if(!out.isNull()){
                    out.save(filename, NULL, dialog.quality);
                }
            }else if(dialog.sidebyside){
                QImage out = drawSideBySide(ui->imageWidget->imgL, ui->imageWidget->imgR, 0, 0, 0, 0, 1.0f, dialog.mirrorL, dialog.mirrorR);

                if(!out.isNull()){
                    out.save(filename, NULL, dialog.quality);
                }
            }else{
                if(dialog.saveL && dialog.saveR){
                    if(!ui->imageWidget->imgL.isNull()){
                        QString filenameL = filename;
                        ui->imageWidget->imgL.save(filenameL.insert(filenameL.lastIndexOf('.'), 'L'), NULL, dialog.quality);
                    }
                    if(!ui->imageWidget->imgR.isNull()){
                        QString filenameR = filename;
                        ui->imageWidget->imgR.save(filenameR.insert(filenameR.lastIndexOf('.'), 'R'), NULL, dialog.quality);
                    }
                }else if(dialog.saveL){
                    if(!ui->imageWidget->imgL.isNull()){
                        ui->imageWidget->imgL.save(filename, NULL, dialog.quality);
                    }
                }else if(dialog.saveR){
                    if(!ui->imageWidget->imgR.isNull()){
                        ui->imageWidget->imgR.save(filename, NULL, dialog.quality);
                    }
                }
            }
        }
    }
}

void DepthViewWindow::on_actionNo_Mirror_triggered(){
    ui->imageWidget->mode = ImageWidget::SidebySide;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionMirror_Left_triggered(){
    ui->imageWidget->mode = ImageWidget::SidebySideMLeft;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionMirror_Right_triggered(){
    ui->imageWidget->mode = ImageWidget::SidebySideMRight;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionMirror_Both_triggered(){
    ui->imageWidget->mode = ImageWidget::SidebySideMBoth;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionFit_triggered(){
    ui->imageWidget->setZoom(0.0f);
}

void DepthViewWindow::on_actionzoom100_triggered(){
    ui->imageWidget->setZoom(1.0f);
}

void DepthViewWindow::on_actionzoom50_triggered(){
    ui->imageWidget->setZoom(0.5f);
}

void DepthViewWindow::on_actionzoom200_triggered(){
    ui->imageWidget->setZoom(2.0f);
}

void DepthViewWindow::on_actionHorizontal_triggered(){
    ui->imageWidget->mode = ImageWidget::InterlacedHorizontal;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionVertical_triggered(){
    ui->imageWidget->mode = ImageWidget::InterlacedVertical;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionOptions_triggered(){
    SettingsWindow settingsdialog(&settings, this);
    if(settingsdialog.exec() == QDialog::Accepted){
        this->loadSettings();
    }
}

void DepthViewWindow::on_actionAbout_triggered(){
    QMessageBox::about(this, tr("About DepthView"),
                       tr("<html><head/><body>"
                          "<h1>DepthView %1 (%2)</h1>"
                          "<p>DepthView is a basic application for viewing stereo 3D image files.</p>"
                          "<p>DepthView website: <a href=\"https://github.com/chipgw/depthview\">github.com/chipgw/depthview</a></p>"
                          "<p>Please report any bugs at: <a href=\"https://github.com/chipgw/depthview/issues\">github.com/chipgw/depthview/issues</a></p>"
                          "</body></html>").arg(version::getVersionString()).arg(version::git_revision.left(7)));
}

void DepthViewWindow::setRendererFromString(const QString &renderer){
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
    else if(renderer == "Mono, Left"){
        this->on_actionLeft_Image_triggered();
    }
    else if(renderer == "Mono, Right"){
        this->on_actionRight_Image_triggered();
    }
    else{
        this->on_actionFull_Color_triggered();
    }
}

void DepthViewWindow::loadSettings(){
    if(settings.contains("defaultrender")){
        setRendererFromString(settings.value("defaultrender").toString());
    }
    if(settings.contains("startfullscreen")){
        ui->actionFullscreen->setChecked(settings.value("startfullscreen").toBool());
    }
    else{
        ui->actionFullscreen->setChecked(false);
    }
    if(settings.contains("swapLR")){
        ui->actionSwap_Left_Right->setChecked(settings.value("swapLR").toBool());
    }
    else{
        ui->actionSwap_Left_Right->setChecked(false);
    }
    if(settings.contains("showmenubar")){
        ui->actionShow_MenuBar->setChecked(settings.value("showmenubar").toBool());
    }
    else{
        ui->actionShow_MenuBar->setChecked(true);
    }
    if(settings.contains("disabledragdrop")){
        setAcceptDrops(!settings.value("disabledragdrop").toBool());
    }
    else{
        setAcceptDrops(true);
    }
    if(settings.contains("startupdirectory") && currentFile.isEmpty() && !settings.value("startupdirectory").toString().isEmpty()){
        QDir::setCurrent(settings.value("startupdirectory").toString());
    }
}

void DepthViewWindow::on_actionCheckerboard_triggered(){
    ui->imageWidget->mode = ImageWidget::Checkerboard;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionLeft_Image_triggered(){
    ui->imageWidget->mode = ImageWidget::MonoLeft;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionRight_Image_triggered(){
    ui->imageWidget->mode = ImageWidget::MonoRight;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionFirst_triggered(){
    QStringList entryList = QDir::current().entryList(fileFilters);
    if(!entryList.isEmpty()){
        loadImage(entryList[0]);
    }
}

void DepthViewWindow::on_actionLast_triggered(){
    QStringList entryList = QDir::current().entryList(fileFilters);
    if(!entryList.isEmpty()){
        loadImage(entryList[entryList.count() - 1]);
    }
}

void DepthViewWindow::on_actionZoomIn_triggered(){
    ui->imageWidget->zoomIn();
}

void DepthViewWindow::on_actionZoomOut_triggered(){
    ui->imageWidget->zoomOut();
}

void DepthViewWindow::on_actionSwap_Left_Right_toggled(bool val){
    ui->imageWidget->swapLR = val;
    ui->imageWidget->repaint();
}

void DepthViewWindow::parseCommandLine(const QStringList &args){
    bool loaded = !this->currentFile.isEmpty();

    for(QStringListIterator i(args); i.hasNext();){
        const QString &arg = i.next();
        if(arg == "--fullscreen"){
            ui->actionFullscreen->setChecked(true);
        }else if(arg == "--startdir"){
            if(i.hasNext()){
                QDir::setCurrent(i.next());
            }else{
                qDebug() << "WARNING: argument --startdir passed with no argument after it!";
            }
        }else if(arg == "--renderer"){
            if(i.hasNext()){
                setRendererFromString(i.next());
            }else{
                qDebug() << "WARNING: argument --renderer passed with no argument after it!";
            }
        }else if(!loaded){
            loaded = loadImage(arg);
        }
    }
    if(!loaded){
        showLoadImageDialog();
    }
}

void DepthViewWindow::dragEnterEvent(QDragEnterEvent *event){
    if (event->mimeData()->hasUrls()){
        foreach(QUrl url, event->mimeData()->urls()){
            QFileInfo info(url.toLocalFile());
            if(info.exists() && (info.suffix().toLower() == "jps" || info.suffix().toLower() == "pns")){
                return event->acceptProposedAction();
            }
        }
    }
}

void DepthViewWindow::dropEvent(QDropEvent *event){
    if (event->mimeData()->hasUrls()){
        foreach(QUrl url, event->mimeData()->urls()){
            if(loadImage(url.toLocalFile())) break;
        }
        event->acceptProposedAction();
    }
}
