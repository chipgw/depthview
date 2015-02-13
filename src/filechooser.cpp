#include "include/filechooser.h"
#include <QFileDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

FileChooser::FileChooser(const QString &filenameFilter, const QString &defaultDir, QWidget *parent, QString text) :
    QWidget(parent), filter(filenameFilter), dir(defaultDir){
    layout = new QHBoxLayout;
    setLayout(layout);

    label = new QLabel(text, this);
    filename = new QLineEdit(this);
    browse = new QPushButton("Browse", this);

    connect(browse, SIGNAL(clicked()), this, SLOT(on_BrowseButton_clicked()));

    layout->addWidget(label);
    layout->addWidget(filename);
    layout->addWidget(browse);
}

void FileChooser::on_BrowseButton_clicked(){
    QString name = QFileDialog::getOpenFileName(this, tr("Open File"), filename->text().isEmpty() ? dir : filename->text(), filter);
    if(!name.isEmpty()){
        filename->setText(name);
    }
}
