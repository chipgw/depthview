#ifndef AUTOWIZARDPAGE_H
#define AUTOWIZARDPAGE_H

#include <QWizardPage>

class AutoWizardPage : public QWizardPage {
    Q_OBJECT
public:
    explicit AutoWizardPage(QLayout *l = 0, QWidget *parent = 0);

    void addOption(const QString &name, QWidget *widget);

private:
    QLayout *layout;
};

#endif // AUTOWIZARDPAGE_H
