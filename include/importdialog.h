#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <QDialog>
#include <QDir>

namespace Ui {
class ImportDialog;
}

class ImportDialog : public QDialog {
    const QDir &currentDir;
    Q_OBJECT
public:
    explicit ImportDialog(const QDir &directory, QWidget *parent = NULL);
    ~ImportDialog();

    bool seperateImages;
    QString filenameLeft;
    QString filenameRight;

private slots:
    void on_pushButtonLeftImageBrowse_clicked();
    void on_pushButtonRightImageBrowse_clicked();
    void accept();

private:
    Ui::ImportDialog *ui;
};

#endif // IMPORTDIALOG_H
