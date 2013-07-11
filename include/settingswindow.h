#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QSettings>
#include <QAbstractButton>
#include <QMessageBox>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog {
    Q_OBJECT
    
public:
    explicit SettingsWindow(QSettings &Settings, QWidget *parent = 0);
    ~SettingsWindow();
    
    const static QString defaultrender;
    const static QString startfullscreen;
    const static QString swapLR;
    const static QString startupdirectory;
    const static QString showmenubar;
    const static QString filedialogstartup;
    const static QString disabledragdrop;
    const static QString continuouspan;
    const static QString showscrollbars;

private slots:
    void accept();

    void on_buttonBox_clicked(QAbstractButton *button);

    void on_startupDirectoryBrowsePushButton_clicked();

private:
    Ui::SettingsWindow *ui;
    QSettings &settings;
};

#endif // SETTINGSWINDOW_H
