#pragma once

#include <QWizard>
#include <QDir>

class ImportWizard : public QWizard {
    Q_OBJECT

    const QDir &currentDirectory;
public:
    explicit ImportWizard(const QDir &directory, QWidget *parent = 0);

    int nextId() const;

private:
    enum { Page_Intro, Page_SelectFile, Page_SideBySide, Page_TopBottom, Page_Seperate, Page_Conclusion };

    QWizardPage *createIntroPage();
    QWizardPage *createSelectFilePage();
    QWizardPage *createSideBySidePage();
    QWizardPage *createTopBottomPage();
    QWizardPage *createSeperatePage();
    QWizardPage *createConclusionPage();
};
