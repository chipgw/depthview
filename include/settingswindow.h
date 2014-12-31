#pragma once

#include <QDialog>
#include <QSettings>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog {
    Q_OBJECT
public:
    explicit SettingsWindow(QSettings &Settings, QWidget *parent = NULL);
    ~SettingsWindow();
    
    const static QString defaultrender;
    const static QString rememberwindow;
    const static QString startfullscreen;
    const static QString swapLR;
    const static QString startupdirectory;
    const static QString showmenubar;
    const static QString filedialogstartup;
    const static QString disabledragdrop;
    const static QString continuouspan;
    const static QString showscrollbars;
    const static QString smoothscaling;
    const static QString panbuttons;

private slots:
    void accept();

    void restoreDefaults();

    void on_startupDirectoryBrowsePushButton_clicked();

    void on_associateWithFilesPushButton_clicked();

private:
    Ui::SettingsWindow *ui;
    QSettings &settings;
};
