#pragma once

#include <QWizardPage>

/* A simple class to put controls on a QWizardPage. */
class AutoWizardPage : public QWizardPage {
    Q_OBJECT
public:
    explicit AutoWizardPage(QLayout *l = 0, QWidget *parent = 0);

    void addOption(const QString &name, QWidget *widget, bool addWidget = true);

private:
    QLayout *layout;
};
