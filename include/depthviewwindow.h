#ifndef DEPTHVIEWWINDOW_H
#define DEPTHVIEWWINDOW_H

#include "imagewidget.h"
#include <QMainWindow>
#include <QSettings>

namespace Ui {
    class DepthViewWindow;
}

class DepthViewWindow : public QMainWindow {
    Q_OBJECT
public:
    QStringList fileFilters;
    QString currentFile;
    QSettings settings;

    explicit DepthViewWindow(QWidget *parent = 0);
    ~DepthViewWindow();

    bool loadImage(QString filename);
    bool showLoadImageDialog();

    void loadSettings();

    void parseCommandLine(const QStringList &args);

    bool setRendererFromString(const QString &renderer);

private:
    Ui::DepthViewWindow *ui;

    void mouseDoubleClickEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private slots:
    void on_actionFullscreen_toggled(bool val);
    void on_actionShowMenuBar_toggled(bool val);
    void on_actionOpen_triggered();
    void on_actionSaveAs_triggered();
    void on_actionOptions_triggered();
    void on_actionAbout_triggered();
    void on_actionAboutQt_triggered();
    void on_actionSwap_Left_Right_toggled(bool val);

    void on_actionFit_triggered();
    void on_actionzoom200_triggered();
    void on_actionzoom100_triggered();
    void on_actionzoom50_triggered();

    void on_actionAnglaphFullColor_triggered();
    void on_actionAnglaphHalfColor_triggered();
    void on_actionAnglaphGreyscale_triggered();
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

#endif // DEPTHVIEWWINDOW_H
