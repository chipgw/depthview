#include "include/importdialog.h"
#include "ui_importdialog.h"
#include <QFileDialog>

ImportDialog::ImportDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ImportDialog), seperateImages(false){
    ui->setupUi(this);
}

ImportDialog::~ImportDialog(){
    delete ui;
}

void ImportDialog::on_pushButtonLeftImageBrowse_clicked(){
    QString file = QFileDialog::getOpenFileName(this, tr("Open Left Eye Image"), ui->lineEditLeftImage->text());
    if(!file.isEmpty()){
        ui->lineEditLeftImage->setText(file);
    }
}

void ImportDialog::on_pushButtonRightImageBrowse_clicked(){
    QString file = QFileDialog::getOpenFileName(this, tr("Open Right Eye Image"), ui->lineEditRightImage->text());
    if(!file.isEmpty()){
        ui->lineEditRightImage->setText(file);
    }
}

void ImportDialog::accept(){
    if(ui->tabWidget->currentWidget() == ui->tabSeperateImages){
        seperateImages = true;
        filenameLeft = ui->lineEditLeftImage->text();
        filenameRight = ui->lineEditRightImage->text();
    }
    QDialog::accept();
}
