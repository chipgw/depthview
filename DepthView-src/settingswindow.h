#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QSettings>

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

private:
    Ui::SettingsWindow *ui;
    QSettings *settings;
};

#endif // SETTINGSWINDOW_H
