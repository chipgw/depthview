#include "settingswindow.h"
#include "ui_settingswindow.h"
#include "imagewidget.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QMenu>

SettingsWindow::SettingsWindow(QSettings &Settings, QWidget *parent) : QDialog(parent), ui(new Ui::SettingsWindow), settings(Settings){
    ui->setupUi(this);

    ui->defaultRendererComboBox->addItems(ImageWidget::drawModeNames.keys());

    ui->defaultRendererComboBox->setCurrentIndex(ui->defaultRendererComboBox->findText(settings.value(defaultrender).toString()));
    ui->startFullscreenCheckBox->setChecked(settings.value(startfullscreen).toBool());
    ui->swapLeftRightCheckBox->setChecked(settings.value(swapLR).toBool());
    ui->startupDirectoryLineEdit->setText(settings.value(startupdirectory).toString());

    // check if it exists so that it will default to true if it does not.
    if(settings.contains(showmenubar)){
        ui->showMenuBarCheckBox->setChecked(settings.value(showmenubar).toBool());
    }
    if(settings.contains(filedialogstartup)){
        ui->fileDialogStartupCheckBox->setChecked(settings.value(filedialogstartup).toBool());
    }
    if(settings.contains(continuouspan)){
        ui->enableContinuousPanCheckBox->setChecked(settings.value(continuouspan).toBool());
    }
    if(settings.contains(showscrollbars)){
        ui->showScrollbarsCheckBox->setChecked(settings.value(showscrollbars).toBool());
    }

    connect(ui->buttonBox->button(QDialogButtonBox::RestoreDefaults), SIGNAL(clicked()), this, SLOT(restoreDefaults()));

    ui->disableDragDropCheckBox->setChecked(settings.value(disabledragdrop).toBool());

    ui->panButtonMenuPushButton->setMenu(new QMenu());
    ui->panButtonMenuPushButton->menu()->addAction(ui->actionLeft_Mouse);
    ui->panButtonMenuPushButton->menu()->addAction(ui->actionMiddle_Mouse);

    if(settings.contains(panbuttons)){
        Qt::MouseButtons buttons = Qt::MouseButtons(settings.value(panbuttons).toInt());

        ui->actionLeft_Mouse->setChecked(buttons.testFlag(Qt::LeftButton));
        ui->actionMiddle_Mouse->setChecked(buttons.testFlag(Qt::MiddleButton));
    }
}

SettingsWindow::~SettingsWindow(){
    delete ui->panButtonMenuPushButton->menu();
    delete ui;
}

void SettingsWindow::accept(){
    settings.setValue(defaultrender,    ui->defaultRendererComboBox->currentText());
    settings.setValue(startfullscreen,  ui->startFullscreenCheckBox->isChecked());
    settings.setValue(swapLR,           ui->swapLeftRightCheckBox->isChecked());
    settings.setValue(startupdirectory, ui->startupDirectoryLineEdit->text());
    settings.setValue(showmenubar,      ui->showMenuBarCheckBox->isChecked());
    settings.setValue(filedialogstartup,ui->fileDialogStartupCheckBox->isChecked());
    settings.setValue(disabledragdrop,  ui->disableDragDropCheckBox->isChecked());
    settings.setValue(continuouspan,    ui->enableContinuousPanCheckBox->isChecked());
    settings.setValue(showscrollbars,   ui->showScrollbarsCheckBox->isChecked());
    settings.setValue(panbuttons,       int((ui->actionLeft_Mouse->isChecked() ? Qt::LeftButton : 0) | (ui->actionMiddle_Mouse->isChecked() ? Qt::MiddleButton : 0)));
    QDialog::accept();
}

void SettingsWindow::restoreDefaults(){
    if(QMessageBox::warning(this, tr("Are You Sure?"), tr("Are you sure you wish to reset to default settings?"),
                            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes){
        settings.clear();

        ui->defaultRendererComboBox->setCurrentIndex(-1);
        ui->startFullscreenCheckBox->setChecked(false);
        ui->swapLeftRightCheckBox->setChecked(false);
        ui->showMenuBarCheckBox->setChecked(true);
        ui->fileDialogStartupCheckBox->setChecked(true);
        ui->disableDragDropCheckBox->setChecked(false);
        ui->startupDirectoryLineEdit->setText("");
        ui->enableContinuousPanCheckBox->setChecked(true);
        ui->showScrollbarsCheckBox->setChecked(true);
        ui->actionLeft_Mouse->setChecked(true);
        ui->actionMiddle_Mouse->setChecked(true);
    }
}

void SettingsWindow::on_startupDirectoryBrowsePushButton_clicked(){
    QString directory = QFileDialog::getExistingDirectory(this, tr("Choose Directory"), ui->startupDirectoryLineEdit->text());
    if(!directory.isEmpty()){
        ui->startupDirectoryLineEdit->setText(directory);
    }
}

const QString SettingsWindow::defaultrender     = "defaultrender";
const QString SettingsWindow::startfullscreen   = "startfullscreen";
const QString SettingsWindow::swapLR            = "swapLR";
const QString SettingsWindow::startupdirectory  = "startupdirectory";
const QString SettingsWindow::showmenubar       = "showmenubar";
const QString SettingsWindow::filedialogstartup = "filedialogstartup";
const QString SettingsWindow::disabledragdrop   = "disabledragdrop";
const QString SettingsWindow::continuouspan     = "continuouspan";
const QString SettingsWindow::showscrollbars    = "showscrollbars";
const QString SettingsWindow::panbuttons        = "panbuttons";
