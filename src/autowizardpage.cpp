#include "include/autowizardpage.h"
#include <QVBoxLayout>

AutoWizardPage::AutoWizardPage(QLayout *l, QWidget *parent) : QWizardPage(parent) , layout(l) {
    /* Default to vertical layout */
    if(layout == nullptr)
        layout = new QVBoxLayout;

    setLayout(layout);
}

void AutoWizardPage::addOption(const QString &name, QWidget *widget, bool addWidget){
    if(addWidget)
        layout->addWidget(widget);

    registerField(name, widget);
}
