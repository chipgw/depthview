#include "include/autowizardpage.h"
#include <QVBoxLayout>

AutoWizardPage::AutoWizardPage(QLayout *l, QWidget *parent) : QWizardPage(parent) , layout(l) {
    /* default to vertical layout */
    if(layout == NULL){
        layout = new QVBoxLayout;
    }
    setLayout(layout);
}

void AutoWizardPage::addOption(const QString &name, QWidget *widget){
    layout->addWidget(widget);

    registerField(name, widget);
}
