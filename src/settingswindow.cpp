#include "settingswindow.h"
#include "ui_settingswindow.h"
#include <QFileDialog>

SettingsWindow::SettingsWindow(QSettings &Settings, QWidget *parent) : QDialog(parent), ui(new Ui::SettingsWindow), settings(Settings){
    ui->setupUi(this);

    ui->defaultRendererComboBox->addItem("Anglaph, Full Color");
    ui->defaultRendererComboBox->addItem("Anglaph, Half Color");
    ui->defaultRendererComboBox->addItem("Anglaph, Greyscale");

    ui->defaultRendererComboBox->addItem("Side by Side, No Mirror");
    ui->defaultRendererComboBox->addItem("Side by Side, Mirror Left");
    ui->defaultRendererComboBox->addItem("Side by Side, Mirror Right");
    ui->defaultRendererComboBox->addItem("Side by Side, Mirror Both");

    ui->defaultRendererComboBox->addItem("Top/Bottom, No Mirror");
    ui->defaultRendererComboBox->addItem("Top/Bottom, Mirror Top");
    ui->defaultRendererComboBox->addItem("Top/Bottom, Mirror Bottom");
    ui->defaultRendererComboBox->addItem("Top/Bottom, Mirror Both");

    ui->defaultRendererComboBox->addItem("Interlaced, Horizontal");
    ui->defaultRendererComboBox->addItem("Interlaced, Vertical");

    ui->defaultRendererComboBox->addItem("Checkerboard");

    ui->defaultRendererComboBox->addItem("Mono, Left");
    ui->defaultRendererComboBox->addItem("Mono, Right");

    ui->defaultRendererComboBox->setCurrentIndex(ui->defaultRendererComboBox->findText(settings.value("defaultrender").toString()));
    ui->startFullscreenCheckBox->setChecked(settings.value("startfullscreen").toBool());
    ui->swapLeftRightCheckBox->setChecked(settings.value("swapLR").toBool());
    ui->startupDirectoryLineEdit->setText(settings.value("startupdirectory").toString());

    // check if it exists so that it will default to true if it does not.
    if(settings.contains("showmenubar")){
        ui->showMenuBarCheckBox->setChecked(settings.value("showmenubar").toBool());
    }
    if(settings.contains("filedialogstartup")){
        ui->fileDialogStartupCheckBox->setChecked(settings.value("filedialogstartup").toBool());
    }

    ui->disableDragDropCheckBox->setChecked(settings.value("disabledragdrop").toBool());
}

SettingsWindow::~SettingsWindow(){
    delete ui;
}

void SettingsWindow::accept(){
    settings.setValue("defaultrender", ui->defaultRendererComboBox->currentText());
    settings.setValue("startfullscreen", ui->startFullscreenCheckBox->isChecked());
    settings.setValue("swapLR", ui->swapLeftRightCheckBox->isChecked());
    settings.setValue("startupdirectory", ui->startupDirectoryLineEdit->text());
    settings.setValue("showmenubar", ui->showMenuBarCheckBox->isChecked());
    settings.setValue("filedialogstartup", ui->fileDialogStartupCheckBox->isChecked());
    settings.setValue("disabledragdrop", ui->disableDragDropCheckBox->isChecked());
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
            settings.clear();

            ui->defaultRendererComboBox->setCurrentIndex(-1);
            ui->startFullscreenCheckBox->setChecked(false);
            ui->swapLeftRightCheckBox->setChecked(false);
            ui->showMenuBarCheckBox->setChecked(true);
            ui->fileDialogStartupCheckBox->setChecked(true);
            ui->disableDragDropCheckBox->setChecked(false);
            ui->startupDirectoryLineEdit->setText("");
        }
    }
}

void SettingsWindow::on_startupDirectoryBrowsePushButton_clicked(){
    QString directory = QFileDialog::getExistingDirectory(this, tr("Choose Directory"), ui->startupDirectoryLineEdit->text());
    if(directory != ""){
        ui->startupDirectoryLineEdit->setText(directory);
    }
}
