#ifndef DEPTHVIEWWINDOW_H
#define DEPTHVIEWWINDOW_H

#include <QMainWindow>
#include <QDir>
#include "imagewidget.h"

namespace Ui {
    class DepthViewWindow;
}

class DepthViewWindow : public QMainWindow { Q_OBJECT
public:
    QStringList fileFilters;
    QString currentFile;

    explicit DepthViewWindow(QWidget *parent = 0);
    ~DepthViewWindow();

    bool loadImage(QString filename);
    bool showLoadImageDialog();

private:
    Ui::DepthViewWindow *ui;

    void mouseDoubleClickEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);

private slots:
    void on_actionPrevious_triggered();
    void on_actionNext_triggered();
    void on_actionFullscreen_toggled(bool val);
    void on_actionShow_MenuBar_toggled(bool val);
    void on_actionGreyscale_triggered();
    void on_actionHalf_Color_triggered();
    void on_actionFull_Color_triggered();
    void on_actionOpen_triggered();
    void on_actionExit_triggered();
    void on_actionAbout_Qt_triggered();
                                             void on_actionSave_As_triggered();
                                           };

#endif // DEPTHVIEWWINDOW_H
