#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&thread, SIGNAL(MantaMessage(const QString &)),
           this, SLOT(DisplayMantaMessage(const QString &)));

    thread.start();
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

void MainWindow::DisplayMantaMessage(const QString &text)
{
    ui->statusBar->showMessage(text, 1000);
}
