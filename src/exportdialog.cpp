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
        this->anglaph = true;
        this->colormult = ui->doubleSpinBox->value();
    }else if(ui->tabWidget->currentWidget() == ui->tabSideBySide){
        this->sidebyside = true;
        this->mirrorL = ui->checkBoxMirrorLeft->isChecked();
        this->mirrorR = ui->checkBoxMirrorRight->isChecked();
    }else if(ui->tabWidget->currentWidget() == ui->tabSingle){
        this->saveL = ui->radioButtonLeft->isChecked() || ui->radioButtonBoth->isChecked();
        this->saveR = ui->radioButtonRight->isChecked() || ui->radioButtonBoth->isChecked();
    }
    quality = ui->qualitySpinBox->value();

    QDialog::accept();
}
