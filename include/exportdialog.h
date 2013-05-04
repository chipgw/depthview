#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>

namespace Ui {
class ExportDialog;
}

class ExportDialog : public QDialog{
    Q_OBJECT
    
public:
    int quality;
    bool anglaph;
    bool sidebyside;
    float colormult;
    bool mirrorL;
    bool mirrorR;
    bool saveL;
    bool saveR;

    explicit ExportDialog(QWidget *parent = 0);
    ~ExportDialog();

private slots:
    void accept();

private:
    Ui::ExportDialog *ui;
};

#endif // EXPORTDIALOG_H
