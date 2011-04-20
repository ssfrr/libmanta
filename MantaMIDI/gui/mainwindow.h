#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "mantathread.h"

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

private:
    Ui::MainWindow *ui;

    MantaThread thread;
};

#endif // MAINWINDOW_H
