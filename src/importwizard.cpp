#include "importwizard.h"
#include "autowizardpage.h"
#include "filechooser.h"
#include <QRadioButton>
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>
#include <QFormLayout>
#include <QVariant>

ImportWizard::ImportWizard(const QDir &directory, QWidget *parent) : QWizard(parent), currentDirectory(directory) {
    setPage(Page_Intro, createIntroPage());
    setPage(Page_SelectFile, createSelectFilePage());
    setPage(Page_SideBySide, createSideBySidePage());
    setPage(Page_TopBottom, createTopBottomPage());
    setPage(Page_Seperate, createSeperatePage());
    setPage(Page_Conclusion, createConclusionPage());
}

int ImportWizard::nextId() const{
    switch(currentId()){
    case Page_Intro:
        if(field("seperate").toBool())
            return Page_Seperate;
        return Page_SelectFile;
    case Page_SelectFile:
        if(field("sideBySide").toBool())
            return Page_SideBySide;
        else if(field("topBottom").toBool())
            return Page_TopBottom;
        return Page_SelectFile;
    case Page_SideBySide:
    case Page_TopBottom:
    case Page_Seperate:
        return -1;// Page_Conclusion; // Useless page currently.
    case Page_Conclusion:
    default:
        return -1;
    }
}

/* automatically make field name from variable */
#define addOption(W) page->addOption(#W, W)

QWizardPage* ImportWizard::createIntroPage() {
    AutoWizardPage *page = new AutoWizardPage;

    page->setTitle(tr("Import Image"));
    page->setSubTitle(tr("Choose Import Type"));

    QRadioButton *seperate = new QRadioButton("Seperate Images", page);
    QRadioButton *sideBySide = new QRadioButton("Side by Side", page);
    QRadioButton *topBottom = new QRadioButton("Top Bottom", page);
    seperate->setChecked(true);

    addOption(seperate);
    addOption(sideBySide);
    addOption(topBottom);

    return page;
}

QWizardPage* ImportWizard::createSideBySidePage() {
    AutoWizardPage *page = new AutoWizardPage;

    page->setTitle(tr("Import Image"));
    page->setSubTitle(tr("Side by Side Options"));

    QCheckBox *mirrorL = new QCheckBox("Mirror Left", page);
    QCheckBox *mirrorR = new QCheckBox("Mirror Right", page);

    addOption(mirrorL);
    addOption(mirrorR);

    return page;
}

QWizardPage* ImportWizard::createTopBottomPage() {
    AutoWizardPage *page = new AutoWizardPage;

    page->setTitle(tr("Import Image"));
    page->setSubTitle(tr("Top/Bottom Options"));

    QCheckBox *mirrorT = new QCheckBox("Mirror Top", page);
    QCheckBox *mirrorB = new QCheckBox("Mirror Bottom", page);

    addOption(mirrorT);
    addOption(mirrorB);

    return page;
}

#undef addOption

QWizardPage* ImportWizard::createSelectFilePage() {
    QVBoxLayout *layout = new QVBoxLayout;
    AutoWizardPage *page = new AutoWizardPage(layout);

    page->setTitle(tr("Import Image"));
    page->setSubTitle(tr("Select Image"));

    FileChooser *filename = new FileChooser(tr("Images (*.png *.xpm *.jpg *.bmp)"), currentDirectory.absolutePath(), page);
    layout->addWidget(filename);

    page->addOption("filename*", filename->filename, false);

    return page;
}

QWizardPage* ImportWizard::createSeperatePage() {
    QVBoxLayout *layout = new QVBoxLayout;
    AutoWizardPage *page = new AutoWizardPage(layout);

    page->setTitle(tr("Import Image"));
    page->setSubTitle(tr("Seperate Images Options"));

    FileChooser *filenameL = new FileChooser(tr("Images (*.png *.xpm *.jpg *.bmp)"), currentDirectory.absolutePath(), page, tr("Left Image"));
    FileChooser *filenameR = new FileChooser(tr("Images (*.png *.xpm *.jpg *.bmp)"), currentDirectory.absolutePath(), page, tr("Right Image"));

    layout->addWidget(filenameL);
    layout->addWidget(filenameR);

    page->addOption("filenameL*", filenameL->filename, false);
    page->addOption("filenameR*", filenameR->filename, false);

    return page;
}

QWizardPage* ImportWizard::createConclusionPage() {
    AutoWizardPage *page = new AutoWizardPage;

    page->setTitle(tr("Import Image"));
    page->setSubTitle(tr("Generic Options"));

    return page;
}
