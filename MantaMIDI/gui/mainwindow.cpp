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

void MainWindow::WriteDebugMessage(const QString &text)
{
    ui->debugOutput->append(text);
}

void MainWindow::UpdateLayout(PadLayout padLayout)
{
    options.SetPad_Layout(padLayout);
    int index = ui->padLayoutGroup->checkedId();
    ui->statusBar->showMessage(QString("Button ID: %1").arg(index), 1000);
    thread.ReloadLEDS();
}

void MainWindow::on_layoutRB_HaydenDuet_clicked()
{
    UpdateLayout(plDuet);
}

void MainWindow::on_layoutRB_Honeycomb_clicked()
{
    UpdateLayout(plHoneycomb);
}

void MainWindow::on_layoutRB_Piano_clicked()
{
    UpdateLayout(plPiano);
}

void MainWindow::on_layoutRB_Chromatic_clicked()
{
    UpdateLayout(plChromatic);
}

void MainWindow::on_layoutRB_Custom_clicked()
{
}
