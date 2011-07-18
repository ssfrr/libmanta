#ifndef PADSETTINGSDIALOG_H
#define PADSETTINGSDIALOG_H

#include <QDialog>
#include <QGraphicsScene>
#include "../MantaMidiSettings.h"

namespace Ui {
    class PadSettingsDialog;
}

class PadSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PadSettingsDialog(MantaMidiSettings *options, QWidget *parent = 0);
    ~PadSettingsDialog();

private slots:
    void on_padSelectionSpin_valueChanged(int );
    void on_padChannelSpin_valueChanged(int );

    void on_padNoteSpin_valueChanged(int );

private:
    void SetWidgetsFromPadIndex(int index);

    Ui::PadSettingsDialog *ui;

    QGraphicsScene scene;
    MantaMidiSettings *m_options;
};

#endif // PADSETTINGSDIALOG_H
