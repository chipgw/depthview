#include "exportdialog.h"
#include "ui_exportdialog.h"

ExportDialog::ExportDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ExportDialog), anglaph(false), sidebyside(false){
    ui->setupUi(this);
}

ExportDialog::~ExportDialog(){
    delete ui;
}

void ExportDialog::accept(){
    if(ui->tabWidget->currentWidget() == ui->tabAnglaph){
        anglaph = true;
        colormult = ui->doubleSpinBox->value();
    }else if(ui->tabWidget->currentWidget() == ui->tabSideBySide){
        sidebyside = true;
        mirrorL = ui->checkBoxMirrorLeft->isChecked();
        mirrorR = ui->checkBoxMirrorRight->isChecked();
    }else if(ui->tabWidget->currentWidget() == ui->tabSingle){
        saveL = ui->radioButtonLeft->isChecked() || ui->radioButtonBoth->isChecked();
        saveR = ui->radioButtonRight->isChecked() || ui->radioButtonBoth->isChecked();
    }
    quality = ui->qualitySpinBox->value();

    QDialog::accept();
}
