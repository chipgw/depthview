#include "include/importdialog.h"
#include "ui_importdialog.h"
#include <QFileDialog>
#include <QMessageBox>

ImportDialog::ImportDialog(const QDir &directory, QWidget *parent) : QDialog(parent), ui(new Ui::ImportDialog), seperateImages(false), currentDir(directory) {
    ui->setupUi(this);
}

ImportDialog::~ImportDialog(){
    delete ui;
}

void ImportDialog::on_pushButtonLeftImageBrowse_clicked(){
    QString file = QFileDialog::getOpenFileName(this, tr("Open Left Eye Image"), ui->lineEditLeftImage->text().isEmpty() ? currentDir.path() : ui->lineEditLeftImage->text(),
                                                "Image Files (*.bmp *.jpg *.jpeg *.png *.ppm *.tiff *.xbm *.xpm)");
    if(!file.isEmpty()){
        ui->lineEditLeftImage->setText(file);
    }
}

void ImportDialog::on_pushButtonRightImageBrowse_clicked(){
    QString file = QFileDialog::getOpenFileName(this, tr("Open Right Eye Image"), ui->lineEditRightImage->text().isEmpty() ? currentDir.path() : ui->lineEditRightImage->text(),
                                                "Image Files (*.bmp *.jpg *.jpeg *.png *.ppm *.tiff *.xbm *.xpm)");
    if(!file.isEmpty()){
        ui->lineEditRightImage->setText(file);
    }
}

void ImportDialog::accept(){
    if(ui->tabWidget->currentWidget() == ui->tabSeperateImages){
        seperateImages = true;
        filenameLeft = ui->lineEditLeftImage->text();
        filenameRight = ui->lineEditRightImage->text();

        if(filenameLeft.isEmpty() || filenameRight.isEmpty()){
            QMessageBox::warning(this, tr("Error importing!"), tr("Please choose the image files to open."));
            return;
        }
        if(!QFile::exists(filenameLeft) || !QFile::exists(filenameRight)){
            QMessageBox::warning(this, tr("Error importing!"), tr("One or both of the files do not exist."));
            return;
        }
    }
    QDialog::accept();
}
