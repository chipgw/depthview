#pragma once

#include <QWidget>
#include <QLineEdit>

class QPushButton;
class QHBoxLayout;
class QLabel;

class FileChooser : public QWidget{
    Q_OBJECT

    QLabel *label;
    QHBoxLayout *layout;
    QPushButton *browse;

    const QString filter;
    const QString dir;
public:
    QLineEdit *filename;

    explicit FileChooser(const QString &filenameFilter, const QString &defaultDir, QWidget *parent = 0, QString text = "");

signals:

public slots:
    void on_BrowseButton_clicked();
};
