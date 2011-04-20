#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&thread, SIGNAL(MantaConnectedMessage(const QString &)),
           this, SLOT(DisplayConnectionMessage(const QString &)));
    connect(&thread, SIGNAL(UpdateStatusMessage(const QString &)),
            this, SLOT(DisplayStatusMessage(const QString &)));

    thread.start();
}

MainWindow::~MainWindow()
{
    thread.terminate();
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

void MainWindow::DisplayConnectionMessage(const QString &text)
{
    ui->mantaConnectedLbl->setText(text);
}

void MainWindow::DisplayStatusMessage(const QString &text)
{
    ui->statusBar->showMessage(text, 1000);
}
