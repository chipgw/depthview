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

    ui->defaultRendererComboBox->setCurrentIndex(ui->defaultRendererComboBox->findText(settings->value("defaultrender").toString()));
    ui->startFullscreenCheckBox->setChecked(settings->value("startfullscreen").toBool());
}

SettingsWindow::~SettingsWindow(){
    delete ui;
}

void SettingsWindow::accept(){
    settings->setValue("defaultrender", ui->defaultRendererComboBox->currentText());
    settings->setValue("startfullscreen", ui->startFullscreenCheckBox->checkState() == Qt::Checked);
    QDialog::accept();
}
