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

    thread.Setup(&options);
    thread.start();
}

MainWindow::~MainWindow()
{
    thread.terminate();
    delete ui;
}

void MainWindow::on_checkBox_clicked()
{
    bool bStatus = ui->checkBox->isChecked();
    QString qMessage = QString("UseVelocity: %1").arg(bStatus);

    options.SetUseVelocity(bStatus);
    ui->statusBar->showMessage(qMessage, 1000);
}

void MainWindow::DisplayConnectionMessage(const QString &text)
{
    ui->mantaConnectedLbl->setText(text);
}

void MainWindow::DisplayStatusMessage(const QString &text)
{
    ui->statusBar->showMessage(text, 1000);
}

