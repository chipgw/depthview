#include "depthviewwindow.h"
#include "ui_depthviewwindow.h"
#include "importdialog.h"
#include "importwizard.h"
#include "exportwizard.h"
#include "settingswindow.h"
#include "version.h"
#include "renderers.h"
#include <QKeyEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QMimeData>
#include <QUrl>
#include <QPainter>
#include <QShortcut>

#if defined(Q_OS_WIN32)
#include <Windows.h>
#endif

DepthViewWindow::DepthViewWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::DepthViewWindow), currentDir(QDir::current()),
#ifdef DEPTHVIEW_PORTABLE
    settings(QApplication::applicationDirPath() + "/DepthView.conf", QSettings::IniFormat)
#else
    settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(), QApplication::applicationName())
#endif
{
#ifndef DEPTHVIEW_PORTABLE
    if(!settings.allKeys().size()){
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
            }
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
    connect(ui->actionSmooth_Scaling,  SIGNAL(toggled(bool)), ui->imageWidget, SLOT(enableSmoothTransform(bool)));

    connect(ui->actionAboutQt, SIGNAL(triggered()), QApplication::instance(), SLOT(aboutQt()));

    ui->menuInterlaced->setEnabled(false);
    ui->actionInterlacedHorizontal->setEnabled(false);
    ui->actionInterlacedVertical->setEnabled(false);
    ui->actionCheckerboard->setEnabled(false);

    QShortcut* altenter = new QShortcut(QKeySequence(Qt::ALT + Qt::Key_Return), this);
    connect(altenter, SIGNAL(activated()), ui->actionFullscreen, SLOT(trigger()));
    QShortcut* f11 = new QShortcut(QKeySequence(Qt::Key_F11), this);
    connect(f11, SIGNAL(activated()), ui->actionFullscreen, SLOT(trigger()));

    loadSettings();
}

DepthViewWindow::~DepthViewWindow(){
    settings.beginGroup("Window");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
    settings.endGroup();

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
            return true;
        }
        currentFile.clear();
        setWindowTitle("DepthView");
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

        ui->menubar->hide();
        ui->actionShowMenuBar->setEnabled(false);

        ui->menuInterlaced->setEnabled(true);
        ui->actionInterlacedHorizontal->setEnabled(true);
        ui->actionInterlacedVertical->setEnabled(true);
        ui->actionCheckerboard->setEnabled(true);
    }else{
        setWindowState(windowState() & ~Qt::WindowFullScreen);

        ui->actionShowMenuBar->setEnabled(true);
        if(ui->actionShowMenuBar->isChecked()){
            ui->menubar->show();
        }

        ui->menuInterlaced->setEnabled(false);
        ui->actionInterlacedHorizontal->setEnabled(false);
        ui->actionInterlacedVertical->setEnabled(false);
        ui->actionCheckerboard->setEnabled(false);
    }
}

void DepthViewWindow::on_actionNext_triggered(){
    QStringList entryList = currentDir.entryList();
    if(!entryList.empty()){
        int index = entryList.indexOf(currentFile);
        do{
            ++index;
            if(index < 0){
                index = entryList.count() - 1;
            }else if(index >= entryList.count()){
                index = 0;
            }
        }while(!loadImage(entryList[index]));
    }
}

void DepthViewWindow::on_actionPrevious_triggered(){
    QStringList entryList = currentDir.entryList();
    if(!entryList.empty()){
        int index = entryList.indexOf(currentFile);
        do{
            --index;
            if(index < 0){
                index = entryList.count() - 1;
            }else if(index >= entryList.count()){
                index = 0;
            }
        }while(!loadImage(entryList[index]));
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
    default: break;
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
    default: break;
    }
}

void DepthViewWindow::on_actionImport_triggered(){
    ImportWizard dialog(currentDir, this);

    if(dialog.exec() != QDialog::Accepted) return;

    if(dialog.field("seperate").toBool()){
        QImage imageL(dialog.field("filenameL").toString());
        QImage imageR(dialog.field("filenameR").toString());
        if(imageL.isNull() || imageR.isNull()){
            QMessageBox::warning(this, tr("Error Importing!"), tr("Error loading image files!"));
            return;
        }
        if(imageL.size() != imageR.size()){
            QMessageBox::warning(this, tr("Error Importing!"), tr("Image sizes are not the same!"));
            return;
        }

        ui->imageWidget->imgL = imageL;
        ui->imageWidget->imgR = imageR;
        ui->imageWidget->updateImages();

        setWindowTitle(tr("[ Imported ]"));
    }
}

void DepthViewWindow::on_actionSaveAs_triggered(){
    if(ui->imageWidget->imgR.isNull() || ui->imageWidget->imgL.isNull()) return;

    QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), currentDir.path(), tr("Stereo Image Files (*.jps *.pns)"));

    if(filename.isNull()) return;

    if(filename.contains(".jps", Qt::CaseInsensitive)){
        QImage out(ui->imageWidget->imgL.width() + ui->imageWidget->imgR.width(), ui->imageWidget->imgL.height(), QImage::Format_RGB32);
        QPainter paint(&out);
        drawSideBySide(ui->imageWidget->pixmapL, ui->imageWidget->pixmapR, 0, 0, paint);

        if(!out.isNull()){
            out.save(filename, "JPG");
        }
    }else if(filename.contains(".pns", Qt::CaseInsensitive)){
        QImage out(ui->imageWidget->imgL.width() + ui->imageWidget->imgR.width(), ui->imageWidget->imgL.height(), QImage::Format_RGB32);
        QPainter paint(&out);
        drawSideBySide(ui->imageWidget->pixmapL, ui->imageWidget->pixmapR, 0, 0, paint);

        if(!out.isNull()){
            out.save(filename, "PNG");
        }
    }
}

void DepthViewWindow::on_actionExport_triggered() {
    if(ui->imageWidget->imgR.isNull() || ui->imageWidget->imgL.isNull()) return;

    QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), currentDir.path(), tr("Image Files (*.bmp *.jpg *.jpeg *.png *.ppm *.tiff *.xbm *.xpm)"));

    if(filename.isNull()) return;

    ExportWizard dialog(this);

    if(dialog.exec() != QDialog::Accepted) return;

    int quality = dialog.field("quality").toInt();

    if(dialog.field("anglaph").toBool()){
        QImage out;

        if(dialog.field("fullColor").toBool()){
            out = drawAnglaph(ui->imageWidget->imgL, ui->imageWidget->imgR);
        }else if(dialog.field("halfColor").toBool()){
            out = drawAnglaphHalf(ui->imageWidget->imgL, ui->imageWidget->imgR);
        }else{
            out = drawAnglaphGrey(ui->imageWidget->imgL, ui->imageWidget->imgR);
        }

        if(!out.isNull()){
            out.save(filename, NULL, quality);
        }
    }else if(dialog.field("sideBySide").toBool()){
        QImage out(ui->imageWidget->imgL.width() + ui->imageWidget->imgR.width(), ui->imageWidget->imgL.height(), QImage::Format_RGB32);
        QPainter paint(&out);
        drawSideBySide(ui->imageWidget->pixmapL, ui->imageWidget->pixmapR, 0, 0, paint, 1.0, dialog.field("mirrorL").toBool(), dialog.field("mirrorR").toBool());

        if(!out.isNull()){
            out.save(filename, NULL, quality);
        }
    }else if(dialog.field("topBottom").toBool()){
        QImage out(ui->imageWidget->imgL.width(), ui->imageWidget->imgL.height() + ui->imageWidget->imgR.height(), QImage::Format_RGB32);
        QPainter paint(&out);
        drawTopBottom(ui->imageWidget->pixmapL, ui->imageWidget->pixmapR, 0, 0, paint, 1.0, dialog.field("mirrorT").toBool(), dialog.field("mirrorB").toBool());

        if(!out.isNull()){
            out.save(filename, NULL, quality);
        }
    }else if(dialog.field("exportBoth").toBool()){
        QString filenameL = filename;
        ui->imageWidget->imgL.save(filenameL.insert(filenameL.lastIndexOf('.'), 'L'), NULL, quality);

        QString filenameR = filename;
        ui->imageWidget->imgR.save(filenameR.insert(filenameR.lastIndexOf('.'), 'R'), NULL, quality);
    }else if(dialog.field("exportL").toBool()){
        ui->imageWidget->imgL.save(filename, NULL, quality);
    }else if(dialog.field("exportR").toBool()){
        ui->imageWidget->imgR.save(filename, NULL, quality);
    }
}

void DepthViewWindow::on_actionFit_triggered(){
    ui->imageWidget->setZoom(0.0);
}

void DepthViewWindow::on_actionzoom100_triggered(){
    ui->imageWidget->setZoom(1.0);
}

void DepthViewWindow::on_actionzoom50_triggered(){
    ui->imageWidget->setZoom(0.5);
}

void DepthViewWindow::on_actionzoom200_triggered(){
    ui->imageWidget->setZoom(2.0);
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
        ui->imageWidget->setRenderMode(ImageWidget::drawModeNames[renderer]);
        return true;
    }
    return false;
}

void DepthViewWindow::loadSettings(){
    if(settings.contains(SettingsWindow::defaultrender)){
        setRenderModeFromString(settings.value(SettingsWindow::defaultrender).toString());
    }
    if(settings.contains(SettingsWindow::rememberwindow) && settings.value(SettingsWindow::rememberwindow).toBool()){
        settings.beginGroup("Window");
        restoreGeometry(settings.value("geometry").toByteArray());
        restoreState(settings.value("state").toByteArray());
        settings.endGroup();

        if(windowState().testFlag(Qt::WindowFullScreen)){
            /* hides the menubar and makes toggling fullscreen work right */
            ui->actionFullscreen->setChecked(true);
        }
    }else if(settings.contains(SettingsWindow::startfullscreen)){
        ui->actionFullscreen->setChecked(settings.value(SettingsWindow::startfullscreen).toBool());
    }else{
        ui->actionFullscreen->setChecked(false);

        /* keeps the menubar from enabling in fullscreen mode */
        if(settings.contains(SettingsWindow::showmenubar)){
            ui->actionShowMenuBar->setChecked(settings.value(SettingsWindow::showmenubar).toBool());
        }else{
            ui->actionShowMenuBar->setChecked(true);
        }
    }

    ui->actionSwap_Left_Right->setChecked(settings.contains(SettingsWindow::swapLR) ? settings.value(SettingsWindow::swapLR).toBool() : false);
    setAcceptDrops(settings.contains(SettingsWindow::disabledragdrop) ? !settings.value(SettingsWindow::disabledragdrop).toBool() : true);
    ui->imageWidget->enableContinuousPan(settings.contains(SettingsWindow::continuouspan) ? settings.value(SettingsWindow::continuouspan).toBool() : true);
    ui->actionShow_Scrollbars->setChecked(settings.contains(SettingsWindow::showscrollbars) ? settings.value(SettingsWindow::showscrollbars).toBool() : true);
    ui->actionSmooth_Scaling->setChecked(settings.contains(SettingsWindow::smoothscaling) ? settings.value(SettingsWindow::smoothscaling).toBool() : false);

    if(settings.contains(SettingsWindow::startupdirectory) && currentFile.isEmpty() && currentDir.absolutePath() == QDir::homePath()){
        currentDir.cd(settings.value(SettingsWindow::startupdirectory).toString());
    }
    if(settings.contains(SettingsWindow::panbuttons)){
        ui->imageWidget->setPanButtons(Qt::MouseButtons(settings.value(SettingsWindow::panbuttons).toInt()));
    }
    if(settings.contains("lastrun")){
        settings.remove("lastrun");
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
                    warning.append(tr("<p>Invalid directory \"%1\" passed to \"--startdir\" argument!</p>").arg(dir));
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
        }else if(arg == "--register"){
            /* handled in main */
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

void DepthViewWindow::registerFileTypes(){
    QString error;

#if defined(Q_OS_WIN32)
    /* TODO - handle errors properly */
    HKEY jpsKey;
    HKEY pnsKey;
    HKEY progKey;
    HKEY cmdKey;

    LPCTSTR progID = TEXT("chipgw.DepthView.1.05");

    if(RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Classes\\.jps"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &jpsKey, NULL) == ERROR_SUCCESS){
        if(RegSetValueEx(jpsKey, NULL, 0, REG_SZ, LPBYTE(progID), strlen(progID)) != ERROR_SUCCESS) {
            error += "<p>Error setting .jps ProgID!</p>";
        }
        RegCloseKey(jpsKey);
    } else {
        error += "<p>Error creating .jps key!</p>";
    }

    if(RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Classes\\.pns"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &pnsKey, NULL) == ERROR_SUCCESS){
        if(RegSetValueEx(pnsKey, NULL, 0, REG_SZ, LPBYTE(progID), strlen(progID)) != ERROR_SUCCESS) {
            error += "<p>Error setting .pns ProgID!</p>";
        }
        RegCloseKey(pnsKey);
    } else {
        error += "<p>Error creating .pns key!</p>";
    }

    LPCTSTR progIDPath = TEXT("Software\\Classes\\chipgw.DepthView.1.05");
    LPCTSTR cmdPath = TEXT("Software\\Classes\\chipgw.DepthView.1.05\\shell\\open\\command");

    if(RegCreateKeyEx(HKEY_CURRENT_USER, progIDPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &progKey, NULL) == ERROR_SUCCESS){
        LPCTSTR desc = TEXT("Stereo 3D Image");
        if(RegSetValueEx(progKey, NULL, 0, REG_SZ, LPBYTE(desc), strlen(desc)) != ERROR_SUCCESS) {
            error += "<p>Error setting description!</p>";
        }
        RegCloseKey(progKey);
    } else {
        error += "<p>Error creating command key!</p>";
    }

    if(RegCreateKeyEx(HKEY_CURRENT_USER, cmdPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &cmdKey, NULL) == ERROR_SUCCESS){
        QString command = "\"" + QDir::toNativeSeparators(QApplication::applicationFilePath()) + "\" \"%1\"";
        if(RegSetValueEx(cmdKey, NULL, 0, REG_SZ, LPBYTE(command.toLocal8Bit().constData()), command.size()) != ERROR_SUCCESS) {
            error += "<p>Error setting command!</p>";
        }
        RegCloseKey(cmdKey);
    } else {
        error += "<p>Error creating command key!</p>";
    }

#else
    /* TODO - make other platforms work. */
    error = tr("File association is currently unsupported on your platform!");
#endif

    if(error.isNull()) {
        QMessageBox::information(NULL, tr("Success!"), tr("Successfully associated .jps and .pns files with DepthView."));
    } else {
        QMessageBox::warning(NULL, tr("Error setting file association!"), error);
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
