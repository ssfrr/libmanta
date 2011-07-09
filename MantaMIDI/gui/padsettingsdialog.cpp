#include "padsettingsdialog.h"
#include "ui_padsettingsdialog.h"

#include <QGraphicsRectItem>

PadSettingsDialog::PadSettingsDialog(OptionHolder *options, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PadSettingsDialog),
    m_options(options)
{
    ui->setupUi(this);

    //QGraphicsRectItem *rect = scene.addRect(QRectF(0, 0, 100, 100));

    //QGraphicsItem *item = scene.itemAt(50, 50);
    // item == rect)

    ui->padLayoutView->setScene(&scene);
    ui->padLayoutView->show();
}

PadSettingsDialog::~PadSettingsDialog()
{
    delete ui;
}

void PadSettingsDialog::SetWidgetsFromPadIndex(int index)
{
    int adjIndex = index-1;
    ui->debugLbl->setText(QString("%1").arg(adjIndex));
    if (m_options)
    {
        ui->padChannelSpin->setValue(m_options->GetPad_EventChannel(adjIndex));
        ui->padNoteSpin->setValue(m_options->GetPad_Midi(adjIndex));
        ui->padcolor_on->setCurrentIndex(m_options->GetPad_OnColor(adjIndex));
        ui->padcolor_off->setCurrentIndex(m_options->GetPad_OffColor(adjIndex));
        ui->padcolor_inactive->setCurrentIndex(m_options->GetPad_InactiveColor(adjIndex));
    }
}

void PadSettingsDialog::on_padSelectionSpin_valueChanged(int value)
{
    SetWidgetsFromPadIndex(value);
}
