#ifndef PADSETTINGSDIALOG_H
#define PADSETTINGSDIALOG_H

#include <QDialog>
#include <QGraphicsScene>
#include "../OptionHolder.h"

namespace Ui {
    class PadSettingsDialog;
}

class PadSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PadSettingsDialog(OptionHolder *options, QWidget *parent = 0);
    ~PadSettingsDialog();

private slots:
    void on_padSelectionSpin_valueChanged(int );

private:
    void SetWidgetsFromPadIndex(int index);

    Ui::PadSettingsDialog *ui;

    QGraphicsScene scene;
    OptionHolder *m_options;
};

#endif // PADSETTINGSDIALOG_H
