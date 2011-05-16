#ifndef PADSETTINGSDIALOG_H
#define PADSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
    class PadSettingsDialog;
}

class PadSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PadSettingsDialog(QWidget *parent = 0);
    ~PadSettingsDialog();

private:
    Ui::PadSettingsDialog *ui;
};

#endif // PADSETTINGSDIALOG_H
