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
    ui->imageWidget->addActions(ui->menubar->actions());
    ui->imageWidget->setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(ui->imageWidget, SIGNAL(doubleClicked()), ui->actionFullscreen, SLOT(toggle()));

    connect(ui->actionZoomIn,  SIGNAL(triggered()), ui->imageWidget, SLOT(zoomIn()));
    connect(ui->actionZoomOut, SIGNAL(triggered()), ui->imageWidget, SLOT(zoomOut()));

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(showLoadImageDialog()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));

    this->loadSettings();
}

DepthViewWindow::~DepthViewWindow(){
    settings.setValue("lastrun", QDate::currentDate().toString());
    delete ui;
}

bool DepthViewWindow::loadImage(const QString &filename){
    QFileInfo info(filename);
    if(info.exists() && (info.suffix().toLower() == "jps" || info.suffix().toLower() == "pns")){
        QDir::setCurrent(info.path());
        currentFile = info.fileName();
        if(ui->imageWidget->loadStereoImage(currentFile)){
            this->setWindowTitle(currentFile);
            ui->imageWidget->repaint();
            return true;
        }else{
            currentFile.clear();
            this->setWindowTitle("DepthView");
        }
    }
    return false;
}

bool DepthViewWindow::showLoadImageDialog(){
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Stereo Image Files (*.jps *.pns)"));
    return loadImage(filename);
}

void DepthViewWindow::on_actionAnglaphFullColor_triggered(){
    ui->imageWidget->mode = ImageWidget::AnglaphFull;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionAnglaphHalfColor_triggered(){
    ui->imageWidget->mode = ImageWidget::AnglaphHalf;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionAnglaphGreyscale_triggered(){
    ui->imageWidget->mode = ImageWidget::AnglaphGreyscale;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionSideBySideNoMirror_triggered(){
    ui->imageWidget->mode = ImageWidget::SidebySide;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionSideBySideMirrorLeft_triggered(){
    ui->imageWidget->mode = ImageWidget::SidebySideMLeft;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionSideBySideMirrorRight_triggered(){
    ui->imageWidget->mode = ImageWidget::SidebySideMRight;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionSideBySideMirrorBoth_triggered(){
    ui->imageWidget->mode = ImageWidget::SidebySideMBoth;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionTopBottomNoMirror_triggered(){
    ui->imageWidget->mode = ImageWidget::TopBottom;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionTopBottomMirrorTop_triggered(){
    ui->imageWidget->mode = ImageWidget::TopBottomMTop;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionTopBottomMirrorBottom_triggered(){
    ui->imageWidget->mode = ImageWidget::TopBottomMBottom;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionTopBottomMirrorBoth_triggered(){
    ui->imageWidget->mode = ImageWidget::TopBottomMBoth;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionInterlacedHorizontal_triggered(){
    ui->imageWidget->mode = ImageWidget::InterlacedHorizontal;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionInterlacedVertical_triggered(){
    ui->imageWidget->mode = ImageWidget::InterlacedVertical;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionCheckerboard_triggered(){
    ui->imageWidget->mode = ImageWidget::Checkerboard;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionSingleLeft_triggered(){
    ui->imageWidget->mode = ImageWidget::MonoLeft;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionSingleRight_triggered(){
    ui->imageWidget->mode = ImageWidget::MonoRight;
    ui->imageWidget->repaint();
}

void DepthViewWindow::on_actionShowMenuBar_toggled(bool val){
    ui->menubar->setVisible(val);
}

void DepthViewWindow::on_actionFullscreen_toggled(bool val){
    if(val){
        setWindowState(windowState() | Qt::WindowFullScreen);
        ui->actionShowMenuBar->setChecked(false);
    }else{
        setWindowState(windowState() & ~Qt::WindowFullScreen);

        if(settings.contains(SettingsWindow::showmenubar)){
            ui->actionShowMenuBar->setChecked(settings.value(SettingsWindow::showmenubar).toBool());
        }else{
            ui->actionShowMenuBar->setChecked(true);
        }
    }
}

void DepthViewWindow::on_actionNext_triggered(){
    QStringList entryList = QDir::current().entryList(fileFilters);
    if(!entryList.empty()){
        int index = entryList.indexOf(currentFile) + 1;
        if(index < 0){
            index = entryList.count() - 1;
        }else if(index >= entryList.count()){
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
        }else if(index >= entryList.count()){
            index = 0;
        }
        loadImage(entryList[index]);
    }
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

void DepthViewWindow::mousePressEvent(QMouseEvent *e){
    if(e->buttons() == Qt::XButton2){
        this->on_actionNext_triggered();
    }
    if(e->buttons() == Qt::XButton1){
        this->on_actionPrevious_triggered();
    }
}

void DepthViewWindow::mouseDoubleClickEvent(QMouseEvent *e){
    if(e->button() == Qt::LeftButton){
        ui->actionFullscreen->toggle();
    }
}

void DepthViewWindow::on_actionSaveAs_triggered(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), "",
                                                    tr("Stereo Image Files (*.jps *.pns);;Image Files (*.bmp *.jpg *.jpeg *.png *.ppm *.tiff *.xbm *.xpm)"));

    if(filename.contains(".jps", Qt::CaseInsensitive) || filename.contains(".pns", Qt::CaseInsensitive)){
        QImage out = drawSideBySide(ui->imageWidget->imgL,ui->imageWidget->imgR, 0, 0);

        if(!out.isNull()){
            if(filename.contains(".jps", Qt::CaseInsensitive)){
                out.save(filename, "JPG");
            }
            if(filename.contains(".pns", Qt::CaseInsensitive)){
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

void DepthViewWindow::on_actionOptions_triggered(){
    SettingsWindow settingsdialog(settings, this);
    if(settingsdialog.exec() == QDialog::Accepted){
        this->loadSettings();
    }
}

void DepthViewWindow::on_actionSwap_Left_Right_toggled(bool val){
    ui->imageWidget->swapLR = val;
    ui->imageWidget->repaint();
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

void DepthViewWindow::on_actionAboutQt_triggered(){
    QMessageBox::aboutQt(this);
}

bool DepthViewWindow::setRendererFromString(const QString &renderer){
    if(ImageWidget::drawModeNames.contains(renderer)){
        ui->imageWidget->mode = ImageWidget::drawModeNames.value(renderer);
        ui->imageWidget->repaint();
        return true;
    }
    return false;
}

void DepthViewWindow::loadSettings(){
    if(settings.contains(SettingsWindow::defaultrender)){
        setRendererFromString(settings.value(SettingsWindow::defaultrender).toString());
    }
    if(settings.contains(SettingsWindow::startfullscreen)){
        ui->actionFullscreen->setChecked(settings.value(SettingsWindow::startfullscreen).toBool());
    }else{
        ui->actionFullscreen->setChecked(false);
    }
    if(settings.contains(SettingsWindow::swapLR)){
        ui->actionSwap_Left_Right->setChecked(settings.value(SettingsWindow::swapLR).toBool());
    }else{
        ui->actionSwap_Left_Right->setChecked(false);
    }
    if(settings.contains(SettingsWindow::showmenubar)){
        ui->actionShowMenuBar->setChecked(settings.value(SettingsWindow::showmenubar).toBool());
    }else{
        ui->actionShowMenuBar->setChecked(true);
    }
    if(settings.contains(SettingsWindow::disabledragdrop)){
        setAcceptDrops(!settings.value(SettingsWindow::disabledragdrop).toBool());
    }else{
        setAcceptDrops(true);
    }
    if(settings.contains(SettingsWindow::startupdirectory) && currentFile.isEmpty() && !settings.value(SettingsWindow::startupdirectory).toString().isEmpty()){
        QDir::setCurrent(settings.value(SettingsWindow::startupdirectory).toString());
    }
}

void DepthViewWindow::parseCommandLine(const QStringList &args){
    bool loaded = !this->currentFile.isEmpty();

    QString warning;

    for(QStringListIterator i(args); i.hasNext();){
        const QString &arg = i.next();
        if(arg == "--fullscreen"){
            ui->actionFullscreen->setChecked(true);
        }else if(arg == "--startdir"){
            if(i.hasNext()){
                const QString &dir = i.next();
                if(!QDir::setCurrent(dir)){
                    warning.append(tr("<p>Invalid directory \"\%1\" passed to \"--startdir\" argument!</p>").arg(dir));
                }
            }else{
                warning.append(tr("<p>Argument \"--startdir\" passed with no argument after it!</p>"));
            }
        }else if(arg == "--renderer"){
            if(i.hasNext()){
                const QString &renderer = i.next();
                if(!setRendererFromString(renderer)){
                    warning.append(tr("<p>Invalid renderer \"%1\" passed to \"--renderer\" argument!</p>").arg(renderer));
                }
            }else{
                warning.append(tr("<p>Argument \"--renderer\" passed with no argument after it!</p>"));
            }
        }else if(!loaded){
            loaded = loadImage(arg);
        }
    }
    if(!warning.isEmpty()){
        QMessageBox::warning(this, tr("Warning: Invalid Command Line!"), warning);
    }

    if(!loaded && (!settings.contains(SettingsWindow::filedialogstartup) || settings.value(SettingsWindow::filedialogstartup).toBool())){
        showLoadImageDialog();
    }
}

void DepthViewWindow::dragEnterEvent(QDragEnterEvent *event){
    if(event->mimeData()->hasUrls()){
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

const QStringList DepthViewWindow::fileFilters = QStringList() << "*.jps" << "*.pns";
