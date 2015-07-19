#include "exportwizard.h"
#include "autowizardpage.h"
#include <QRadioButton>
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>
#include <QFormLayout>
#include <QVariant>

ExportWizard::ExportWizard(QWidget *parent) : QWizard(parent) {
    setPage(Page_Intro, createIntroPage());
    setPage(Page_Anglaph, createAnglaphPage());
    setPage(Page_SideBySide, createSideBySidePage());
    setPage(Page_TopBottom, createTopBottomPage());
    setPage(Page_Seperate, createSeperatePage());
    setPage(Page_Conclusion, createConclusionPage());
}

int ExportWizard::nextId() const{
    switch(currentId()){
    case Page_Intro:
        if(field("anglaph").toBool())
            return ExportWizard::Page_Anglaph;
        else if(field("sideBySide").toBool())
            return ExportWizard::Page_SideBySide;
        else if(field("topBottom").toBool())
            return ExportWizard::Page_TopBottom;
        else if(field("seperate").toBool())
            return ExportWizard::Page_Seperate;

        return -1;
    case Page_Anglaph:
    case Page_SideBySide:
    case Page_TopBottom:
    case Page_Seperate:
        return Page_Conclusion;
    case Page_Conclusion:
    default:
        return -1;
    }
}

/* automatically make field name from variable */
#define addOption(W) page->addOption(#W, W)

QWizardPage* ExportWizard::createIntroPage() {
    AutoWizardPage *page = new AutoWizardPage;

    page->setTitle(tr("Export Image"));
    page->setSubTitle(tr("Choose Export Type"));

    QRadioButton *anglaph = new QRadioButton("Anglaph", page);
    QRadioButton *sideBySide = new QRadioButton("Side by Side", page);
    QRadioButton *topBottom = new QRadioButton("Top Bottom", page);
    QRadioButton *seperate = new QRadioButton("Seperate Images", page);
    anglaph->setChecked(true);

    addOption(anglaph);
    addOption(sideBySide);
    addOption(topBottom);
    addOption(seperate);

    return page;
}

QWizardPage* ExportWizard::createAnglaphPage() {
    AutoWizardPage *page = new AutoWizardPage;

    page->setTitle(tr("Export Image"));
    page->setSubTitle(tr("Anglaph Options"));

    QRadioButton *fullColor = new QRadioButton("Full Color", page);
    QRadioButton *halfColor = new QRadioButton("Half Color", page);
    QRadioButton *greyscale = new QRadioButton("Greyscale", page);
    fullColor->setChecked(true);

    addOption(fullColor);
    addOption(halfColor);
    addOption(greyscale);

    return page;
}

QWizardPage* ExportWizard::createSideBySidePage() {
    AutoWizardPage *page = new AutoWizardPage;

    page->setTitle(tr("Export Image"));
    page->setSubTitle(tr("Side by Side Options"));

    QCheckBox *mirrorL = new QCheckBox("Mirror Left", page);
    QCheckBox *mirrorR = new QCheckBox("Mirror Right", page);
    QCheckBox *anamorphic = new QCheckBox("Anamorphic", page);

    addOption(mirrorL);
    addOption(mirrorR);
    addOption(anamorphic);

    return page;
}

QWizardPage* ExportWizard::createTopBottomPage() {
    AutoWizardPage *page = new AutoWizardPage;

    page->setTitle(tr("Export Image"));
    page->setSubTitle(tr("Top/Bottom Options"));

    QCheckBox *mirrorT = new QCheckBox("Mirror Top", page);
    QCheckBox *mirrorB = new QCheckBox("Mirror Bottom", page);
    QCheckBox *anamorphic = new QCheckBox("Anamorphic", page);

    addOption(mirrorT);
    addOption(mirrorB);
    addOption(anamorphic);

    return page;
}

QWizardPage* ExportWizard::createSeperatePage() {
    AutoWizardPage *page = new AutoWizardPage;

    page->setTitle(tr("Export Image"));
    page->setSubTitle(tr("Seperate Images Options"));

    QRadioButton *exportL = new QRadioButton("Export Left Image", page);
    QRadioButton *exportR = new QRadioButton("Export Right Image", page);
    QRadioButton *exportBoth = new QRadioButton("Export Both Images (as seperate files)", page);
    exportL->setChecked(true);

    addOption(exportL);
    addOption(exportR);
    addOption(exportBoth);

    return page;
}

QWizardPage* ExportWizard::createConclusionPage() {
    QFormLayout *layout = new QFormLayout;
    AutoWizardPage *page = new AutoWizardPage(layout);

    page->setTitle(tr("Export Image"));
    page->setSubTitle(tr("Generic Options"));

    QSpinBox *quality = new QSpinBox(page);
    quality->setMinimum(-1);
    quality->setMaximum(100);
    quality->setValue(-1);

    addOption(quality);

    layout->setWidget(0, QFormLayout::LabelRole, new QLabel("Image Quality"));
    layout->setWidget(0, QFormLayout::FieldRole, quality);

    return page;
}
