#include "depthviewwindow.h"
#include "ui_depthviewwindow.h"
#include "exportdialog.h"
#include "settingswindow.h"
#include "version.h"
#include "renderers.h"
#include <QKeyEvent>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QTime>
#include <QMimeData>
#include <QUrl>

DepthViewWindow::DepthViewWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::DepthViewWindow), currentDir(QDir::current()),
#ifdef DEPTHVIEW_PORTABLE
    settings(QApplication::applicationDirPath() + "/DepthView.conf", QSettings::IniFormat)
#else
    settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(), QApplication::applicationName())
#endif
{
#ifndef DEPTHVIEW_PORTABLE
    if(!settings.allKeys().size()){
        qDebug() << "settings empty, checking old settings location.";
        QSettings oldSettings("DepthView","DepthView");

        if(oldSettings.allKeys().size()){
            QMessageBox msgBox(this);
            msgBox.setWindowTitle(tr("Migrate Settings?"));
            msgBox.setText(tr("The location for storing settings has changed in DepthView 1.05, Do you want to migrate the old settings?"));

            QPushButton *deleteButton = msgBox.addButton(tr("Migrate && Delete Old Settings"), QMessageBox::AcceptRole);
            QPushButton *keepButton = msgBox.addButton(tr("Migrate && Keep Old Settings"), QMessageBox::AcceptRole);
            msgBox.addButton(tr("Don't Migrate"), QMessageBox::RejectRole);

            msgBox.exec();

            if(msgBox.clickedButton() == (QAbstractButton*)deleteButton || msgBox.clickedButton() == (QAbstractButton*)keepButton){
                foreach(QString key, oldSettings.allKeys()){
                    settings.setValue(key, oldSettings.value(key));
                }

                if(msgBox.clickedButton() == (QAbstractButton*)deleteButton){
                    oldSettings.clear();
                }

                qDebug() << "settings migrated.";
            }
        }else{
            qDebug() << "no old settings to migrate.";
        }
    }
#endif

    currentDir.setNameFilters(QStringList() << "*.jps" << "*.pns");

    ui->setupUi(this);
    ui->imageWidget->addActions(ui->menubar->actions());
    ui->imageWidget->setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(ui->actionZoomIn,  SIGNAL(triggered()), ui->imageWidget, SLOT(zoomIn()));
    connect(ui->actionZoomOut, SIGNAL(triggered()), ui->imageWidget, SLOT(zoomOut()));

    connect(ui->actionShowMenuBar, SIGNAL(toggled(bool)), ui->menubar, SLOT(setVisible(bool)));

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(showLoadImageDialog()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));

    connect(ui->actionSwap_Left_Right, SIGNAL(toggled(bool)), ui->imageWidget, SLOT(enableSwapLR(bool)));
    connect(ui->actionShow_Scrollbars, SIGNAL(toggled(bool)), ui->imageWidget, SLOT(showScrollbars(bool)));

    connect(ui->actionAboutQt, SIGNAL(triggered()), QApplication::instance(), SLOT(aboutQt()));

    loadSettings();
}

DepthViewWindow::~DepthViewWindow(){
    settings.setValue("lastrun", QDate::currentDate().toString());
    delete ui;
}

bool DepthViewWindow::loadImage(const QString &filename){
    QFileInfo info(filename);

    if(!info.exists()){
        info.setFile(currentDir.filePath(filename));

        if(!info.exists()){
            return false;
        }
    }

    if(QDir::match(currentDir.nameFilters(), info.fileName())){
        currentDir.cd(info.path());
        currentFile = info.fileName();
        if(ui->imageWidget->loadStereoImage(info.filePath())){
            setWindowTitle(currentFile);
            ui->imageWidget->repaint();
            return true;
        }else{
            currentFile.clear();
            setWindowTitle("DepthView");
        }
    }
    return false;
}

bool DepthViewWindow::showLoadImageDialog(){
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Image"), currentDir.path(), tr("Stereo Image Files (*.jps *.pns)"));
    return loadImage(filename);
}

void DepthViewWindow::on_actionAnglaphFullColor_triggered(){
    ui->imageWidget->setRenderMode(ImageWidget::AnglaphFull);
}

void DepthViewWindow::on_actionAnglaphHalfColor_triggered(){
    ui->imageWidget->setRenderMode(ImageWidget::AnglaphHalf);
}

void DepthViewWindow::on_actionAnglaphGreyscale_triggered(){
    ui->imageWidget->setRenderMode(ImageWidget::AnglaphGreyscale);
}

void DepthViewWindow::on_actionSideBySideNoMirror_triggered(){
    ui->imageWidget->setRenderMode(ImageWidget::SidebySide);
}

void DepthViewWindow::on_actionSideBySideMirrorLeft_triggered(){
    ui->imageWidget->setRenderMode(ImageWidget::SidebySideMLeft);
}

void DepthViewWindow::on_actionSideBySideMirrorRight_triggered(){
    ui->imageWidget->setRenderMode(ImageWidget::SidebySideMRight);
}

void DepthViewWindow::on_actionSideBySideMirrorBoth_triggered(){
    ui->imageWidget->setRenderMode(ImageWidget::SidebySideMBoth);
}

void DepthViewWindow::on_actionTopBottomNoMirror_triggered(){
    ui->imageWidget->setRenderMode(ImageWidget::TopBottom);
}

void DepthViewWindow::on_actionTopBottomMirrorTop_triggered(){
    ui->imageWidget->setRenderMode(ImageWidget::TopBottomMTop);
}

void DepthViewWindow::on_actionTopBottomMirrorBottom_triggered(){
    ui->imageWidget->setRenderMode(ImageWidget::TopBottomMBottom);
}

void DepthViewWindow::on_actionTopBottomMirrorBoth_triggered(){
    ui->imageWidget->setRenderMode(ImageWidget::TopBottomMBoth);
}

void DepthViewWindow::on_actionInterlacedHorizontal_triggered(){
    ui->imageWidget->setRenderMode(ImageWidget::InterlacedHorizontal);
}

void DepthViewWindow::on_actionInterlacedVertical_triggered(){
    ui->imageWidget->setRenderMode(ImageWidget::InterlacedVertical);
}

void DepthViewWindow::on_actionCheckerboard_triggered(){
    ui->imageWidget->setRenderMode(ImageWidget::Checkerboard);
}

void DepthViewWindow::on_actionSingleLeft_triggered(){
    ui->imageWidget->setRenderMode(ImageWidget::MonoLeft);
}

void DepthViewWindow::on_actionSingleRight_triggered(){
    ui->imageWidget->setRenderMode(ImageWidget::MonoRight);
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
    QStringList entryList = currentDir.entryList();
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
    QStringList entryList = currentDir.entryList();
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
    QStringList entryList = currentDir.entryList();
    if(!entryList.isEmpty()){
        loadImage(entryList[0]);
    }
}

void DepthViewWindow::on_actionLast_triggered(){
    QStringList entryList = currentDir.entryList();
    if(!entryList.isEmpty()){
        loadImage(entryList[entryList.count() - 1]);
    }
}

void DepthViewWindow::mousePressEvent(QMouseEvent *e){
    switch(e->button()){
    case Qt::XButton2:
        on_actionNext_triggered();
        break;
    case Qt::XButton1:
        on_actionPrevious_triggered();
        break;
    }
}

void DepthViewWindow::mouseDoubleClickEvent(QMouseEvent *e){
    switch(e->button()){
    case Qt::LeftButton:
        ui->actionFullscreen->toggle();
        break;
    case Qt::MiddleButton:
        on_actionFit_triggered();
        break;
    }
}

void DepthViewWindow::on_actionSaveAs_triggered(){
    if(ui->imageWidget->imgR.isNull() || ui->imageWidget->imgL.isNull()) return;

    QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), currentDir.path(),
                                                    tr("Stereo Image Files (*.jps *.pns);;Image Files (*.bmp *.jpg *.jpeg *.png *.ppm *.tiff *.xbm *.xpm)"));

    if(filename.isNull()) return;

    if(filename.contains(".jps", Qt::CaseInsensitive)){
        QImage out = drawSideBySide(ui->imageWidget->imgL,ui->imageWidget->imgR, 0, 0);

        if(!out.isNull()){
            out.save(filename, "JPG");
        }
    }else if(filename.contains(".pns", Qt::CaseInsensitive)){
        QImage out = drawSideBySide(ui->imageWidget->imgL,ui->imageWidget->imgR, 0, 0);

        if(!out.isNull()){
            out.save(filename, "PNG");
        }
    }else{
        ExportDialog dialog(this);

        if(dialog.exec() != QDialog::Accepted) return;

        if(dialog.anglaph){
            QImage out = drawAnglaph(ui->imageWidget->imgL, ui->imageWidget->imgR, 0, 0, QSize(), 1.0f, dialog.colormult);

            if(!out.isNull()){
                out.save(filename, NULL, dialog.quality);
            }
        }else if(dialog.sidebyside){
            QImage out = drawSideBySide(ui->imageWidget->imgL, ui->imageWidget->imgR, 0, 0, QSize(), 1.0f, dialog.mirrorL, dialog.mirrorR);

            if(!out.isNull()){
                out.save(filename, NULL, dialog.quality);
            }
        }else if(dialog.saveL && dialog.saveR){
            QString filenameL = filename;
            ui->imageWidget->imgL.save(filenameL.insert(filenameL.lastIndexOf('.'), 'L'), NULL, dialog.quality);

            QString filenameR = filename;
            ui->imageWidget->imgR.save(filenameR.insert(filenameR.lastIndexOf('.'), 'R'), NULL, dialog.quality);
        }else if(dialog.saveL){
            ui->imageWidget->imgL.save(filename, NULL, dialog.quality);
        }else if(dialog.saveR){
            ui->imageWidget->imgR.save(filename, NULL, dialog.quality);
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
        loadSettings();
    }
}

void DepthViewWindow::on_actionAbout_triggered(){
    QMessageBox::about(this, tr("About DepthView"),
                       tr("<html><head/><body><h1>DepthView %1"
                      #ifdef DEPTHVIEW_PORTABLE
                          " Portable"
                      #endif
                      #ifndef NDEBUG
                          " Debug"
                      #endif
                          " (%2)</h1><p>DepthView is a basic application for viewing stereo 3D image files.</p>"
                          "<p>DepthView website: <a href=\"https://github.com/chipgw/depthview\">github.com/chipgw/depthview</a></p>"
                          "<p>Please report any bugs at: <a href=\"https://github.com/chipgw/depthview/issues\">github.com/chipgw/depthview/issues</a></p>"
                          "</body></html>").arg(version::getVersionString()).arg(version::git_revision.left(7)));
}

bool DepthViewWindow::setRenderModeFromString(const QString &renderer){
    if(ImageWidget::drawModeNames.contains(renderer)){
        ui->imageWidget->setRenderMode(ImageWidget::drawModeNames.value(renderer));
        return true;
    }
    return false;
}

void DepthViewWindow::loadSettings(){
    if(settings.contains(SettingsWindow::defaultrender)){
        setRenderModeFromString(settings.value(SettingsWindow::defaultrender).toString());
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
    if(settings.contains(SettingsWindow::continuouspan)){
        ui->imageWidget->enableContinuousPan(settings.value(SettingsWindow::continuouspan).toBool());
    }else{
        ui->imageWidget->enableContinuousPan(true);
    }
    if(settings.contains(SettingsWindow::showscrollbars)){
        ui->actionShow_Scrollbars->setChecked(settings.value(SettingsWindow::showscrollbars).toBool());
    }else{
        ui->actionShow_Scrollbars->setChecked(true);
    }
    if(settings.contains(SettingsWindow::startupdirectory) && currentFile.isEmpty() && !settings.value(SettingsWindow::startupdirectory).toString().isEmpty()){
        currentDir.cd(settings.value(SettingsWindow::startupdirectory).toString());
    }
}

void DepthViewWindow::parseCommandLine(const QStringList &args){
    bool loaded = !currentFile.isEmpty();

    QString warning;

    for(QStringListIterator i(args); i.hasNext();){
        const QString &arg = i.next();
        if(arg == "--fullscreen"){
            ui->actionFullscreen->setChecked(true);
        }else if(arg == "--startdir"){
            if(i.hasNext()){
                const QString &dir = i.next();
                if(!currentDir.cd(dir)){
                    warning.append(tr("<p>Invalid directory \"\%1\" passed to \"--startdir\" argument!</p>").arg(dir));
                }
            }else{
                warning.append(tr("<p>Argument \"--startdir\" passed with no argument after it!</p>"));
            }
        }else if(arg == "--renderer"){
            if(i.hasNext()){
                const QString &renderer = i.next();
                if(!setRenderModeFromString(renderer)){
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
            if(info.exists() && QDir::match(currentDir.nameFilters(), info.fileName())){
                return event->acceptProposedAction();
            }
        }
    }
}

void DepthViewWindow::dropEvent(QDropEvent *event){
    if(event->mimeData()->hasUrls()){
        foreach(QUrl url, event->mimeData()->urls()){
            if(loadImage(url.toLocalFile())){
                return event->acceptProposedAction();
            }
        }
    }
}
