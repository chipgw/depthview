#pragma once

#include <QWizard>

class ExportWizard : public QWizard {
    Q_OBJECT
public:
    explicit ExportWizard(QWidget *parent = 0);

    int nextId() const;

private:
    enum { Page_Intro, Page_Anglaph, Page_SideBySide, Page_TopBottom, Page_Seperate, Page_Conclusion };

    QWizardPage *createIntroPage();
    QWizardPage *createAnglaphPage();
    QWizardPage *createSideBySidePage();
    QWizardPage *createTopBottomPage();
    QWizardPage *createSeperatePage();
    QWizardPage *createConclusionPage();
};
