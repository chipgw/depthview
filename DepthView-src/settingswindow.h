#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "common.h"
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
    explicit SettingsWindow(QSettings *Settings, QWidget *parent = 0);
    ~SettingsWindow();
    
private slots:
    void accept();

    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::SettingsWindow *ui;
    QSettings *settings;
};

#endif // SETTINGSWINDOW_H
