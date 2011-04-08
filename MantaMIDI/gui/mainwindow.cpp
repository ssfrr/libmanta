#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_checkBox_clicked()
{
    QString on = "UseVelocityOn";
    QString off = "UseVelocityOff";

    if(ui->checkBox->isChecked())
        ui->statusBar->showMessage(on, 1000);
    else
        ui->statusBar->showMessage(off, 1000);
}
