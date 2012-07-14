#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QSettings *Settings, QWidget *parent) : QDialog(parent), ui(new Ui::SettingsWindow){
    ui->setupUi(this);
    settings = Settings;

    ui->defaultRendererComboBox->addItem("Anglaph, Full Color");
    ui->defaultRendererComboBox->addItem("Anglaph, Half Color");
    ui->defaultRendererComboBox->addItem("Anglaph, Greyscale");

    ui->defaultRendererComboBox->addItem("Side by Side, No Mirror");
    ui->defaultRendererComboBox->addItem("Side by Side, Mirror Left");
    ui->defaultRendererComboBox->addItem("Side by Side, Mirror Right");
    ui->defaultRendererComboBox->addItem("Side by Side, Mirror Both");

    ui->defaultRendererComboBox->addItem("Interlaced, Horizontal");
    ui->defaultRendererComboBox->addItem("Interlaced, Vertical");

    ui->defaultRendererComboBox->addItem("Checkerboard");

    ui->defaultRendererComboBox->addItem("Mono, Left");
    ui->defaultRendererComboBox->addItem("Mono, Right");

    ui->defaultRendererComboBox->setCurrentIndex(ui->defaultRendererComboBox->findText(settings->value("defaultrender").toString()));
    ui->startFullscreenCheckBox->setChecked(settings->value("startfullscreen").toBool());
    ui->smoothZoomCheckBox->setChecked(settings->value("smoothzoom").toBool());
}

SettingsWindow::~SettingsWindow(){
    delete ui;
}

void SettingsWindow::accept(){
    settings->setValue("defaultrender", ui->defaultRendererComboBox->currentText());
    settings->setValue("startfullscreen", ui->startFullscreenCheckBox->checkState() == Qt::Checked);
    settings->setValue("smoothzoom", ui->smoothZoomCheckBox->checkState() == Qt::Checked);
    QDialog::accept();
}

void SettingsWindow::on_buttonBox_clicked(QAbstractButton *button){
    if(button->text() == "Restore Defaults"){
        QMessageBox msgbox(this);
        msgbox.setText("Are you sure you wish to reset to default settings?");
        msgbox.setWindowTitle("Are You Sure?");
        msgbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgbox.setDefaultButton(QMessageBox::Yes);
        msgbox.setIcon(QMessageBox::Warning);

        if(msgbox.exec() == QMessageBox::Yes){
            settings->clear();

            ui->defaultRendererComboBox->setCurrentIndex(-1);
            ui->startFullscreenCheckBox->setChecked(false);
            ui->smoothZoomCheckBox->setChecked(false);
        }
    }
}
