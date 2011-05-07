#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "mantathread.h"
#include "../OptionHolder.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_checkBox_clicked();

    void DisplayConnectionMessage(const QString &text);
    void DisplayStatusMessage(const QString &text);
    void WriteDebugMessage(const QString &text);

    void on_layoutRB_HaydenDuet_clicked();
    void on_layoutRB_Honeycomb_clicked();
    void on_layoutRB_Piano_clicked();
    void on_layoutRB_Chromatic_clicked();
    void on_layoutRB_Custom_clicked();

private:
    void UpdateLayout(PadLayout padLayout);

    Ui::MainWindow *ui;

    OptionHolder options;
    MantaThread thread;
};

#endif // MAINWINDOW_H
