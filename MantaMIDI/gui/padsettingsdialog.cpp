#include "padsettingsdialog.h"
#include "ui_padsettingsdialog.h"

PadSettingsDialog::PadSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PadSettingsDialog)
{
    ui->setupUi(this);
}

PadSettingsDialog::~PadSettingsDialog()
{
    delete ui;
}
