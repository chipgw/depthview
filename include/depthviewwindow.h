#pragma once

#include "imagewidget.h"
#include <QMainWindow>
#include <QSettings>
#include <QDir>

class QCommandLineParser;

namespace Ui {
class DepthViewWindow;
}

class DepthViewWindow : public QMainWindow {
    Q_OBJECT

private:
    Ui::DepthViewWindow *ui;

    void mouseDoubleClickEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);

    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);

    void closeEvent(QCloseEvent* e);

public:
    QDir currentDir;
    QString currentFile;
    QSettings settings;

    explicit DepthViewWindow(QWidget *parent = nullptr);
    ~DepthViewWindow();

    bool loadImage(const QString &filename);

    void loadSettings();

    void parseCommandLine(const QCommandLineParser &args);
    static void registerFileTypes();

    bool setRenderModeFromString(const QString &renderer);

private slots:
    bool showLoadImageDialog();

    void on_actionFullscreen_toggled(bool val);
    void on_actionSaveAs_triggered();
    void on_actionExport_triggered();
    void on_actionImport_triggered();
    void on_actionOptions_triggered();
    void on_actionAbout_triggered();

    void on_actionFit_triggered();
    void on_actionzoom200_triggered();
    void on_actionzoom100_triggered();
    void on_actionzoom50_triggered();

    void on_actionAnaglyphFullColor_triggered();
    void on_actionAnaglyphHalfColor_triggered();
    void on_actionAnaglyphGreyscale_triggered();
    void on_actionSideBySideNoMirror_triggered();
    void on_actionSideBySideMirrorLeft_triggered();
    void on_actionSideBySideMirrorRight_triggered();
    void on_actionSideBySideMirrorBoth_triggered();
    void on_actionTopBottomNoMirror_triggered();
    void on_actionTopBottomMirrorTop_triggered();
    void on_actionTopBottomMirrorBottom_triggered();
    void on_actionTopBottomMirrorBoth_triggered();
    void on_actionInterlacedHorizontal_triggered();
    void on_actionInterlacedVertical_triggered();
    void on_actionCheckerboard_triggered();
    void on_actionSingleLeft_triggered();
    void on_actionSingleRight_triggered();

    void on_actionNext_triggered();
    void on_actionPrevious_triggered();
    void on_actionFirst_triggered();
    void on_actionLast_triggered();
};
