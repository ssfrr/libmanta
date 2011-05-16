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
    if (padSettingsDialog)
        delete padSettingsDialog;
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

void MainWindow::on_padAftertouch_Monophonic_clicked()
{
    options.SetPad_Mode(pvmMonoAftertouch);
}

void MainWindow::on_padAftertouch_Polyphonic_clicked()
{
    options.SetPad_Mode(pvmPolyAftertouch);
}

void MainWindow::on_padAftertouch_PolyphonicController_clicked()
{
    options.SetPad_Mode(pvmPolyContinuous);
}

void MainWindow::on_button1Channel_valueChanged(int value)
{
    options.SetButton_Channel(0, value - 1);
}

void MainWindow::on_button2Channel_valueChanged(int value)
{
    options.SetButton_Channel(1, value - 1);
}

void MainWindow::on_button3Channel_valueChanged(int value)
{
    options.SetButton_Channel(2, value - 1);
}

void MainWindow::on_button4Channel_valueChanged(int value)
{
    options.SetButton_Channel(3, value - 1);
}

void MainWindow::on_button1Note_valueChanged(int value)
{
    options.SetButton_Midi(0, (char)value - 1);
    thread.ReloadLEDS();
}

void MainWindow::on_button2Note_valueChanged(int value)
{
    options.SetButton_Midi(1, (char)value - 1);
    thread.ReloadLEDS();
}

void MainWindow::on_button3Note_valueChanged(int value)
{
    options.SetButton_Midi(2, (char)value - 1);
    thread.ReloadLEDS();
}

void MainWindow::on_button4Note_valueChanged(int value)
{
    options.SetButton_Midi(3, (char)value - 1);
    thread.ReloadLEDS();
}

void MainWindow::on_button1type_conrol_clicked()
{
    options.SetButton_Mode(0, bmController);
}

void MainWindow::on_button1type_note_clicked()
{
    options.SetButton_Mode(0, bmNote);
}

void MainWindow::on_button2type_control_clicked()
{
    options.SetButton_Mode(1, bmController);
}

void MainWindow::on_button2type_note_clicked()
{
    options.SetButton_Mode(1, bmNote);
}

void MainWindow::on_button3type_control_clicked()
{
    options.SetButton_Mode(2, bmController);
}

void MainWindow::on_button3type_note_clicked()
{
    options.SetButton_Mode(2, bmNote);
}

void MainWindow::on_butotn4type_control_clicked()
{
    options.SetButton_Mode(3, bmController);
}

void MainWindow::on_button4type_note_clicked()
{
    options.SetButton_Mode(3, bmNote);
}

void MainWindow::on_button1color_on_currentIndexChanged(int index)
{
    options.SetButton_OnColor(0, (Manta::LEDState)index);
    thread.ReloadLEDS();
}

void MainWindow::on_button1color_off_currentIndexChanged(int index)
{
    options.SetButton_OffColor(0, (Manta::LEDState)index);
    thread.ReloadLEDS();
}

void MainWindow::on_button1color_inactive_currentIndexChanged(int index)
{
    options.SetButton_InactiveColor(0, (Manta::LEDState)index);
    thread.ReloadLEDS();
}

void MainWindow::on_button2color_on_currentIndexChanged(int index)
{
    options.SetButton_OnColor(1, (Manta::LEDState)index);
    thread.ReloadLEDS();
}

void MainWindow::on_button2color_off_currentIndexChanged(int index)
{
    options.SetButton_OffColor(1, (Manta::LEDState)index);
    thread.ReloadLEDS();
}

void MainWindow::on_button2color_inactive_currentIndexChanged(int index)
{
    options.SetButton_InactiveColor(1, (Manta::LEDState)index);
    thread.ReloadLEDS();
}

void MainWindow::on_button3color_on_currentIndexChanged(int index)
{
    options.SetButton_OnColor(2, (Manta::LEDState)index);
    thread.ReloadLEDS();
}

void MainWindow::on_button3color_off_currentIndexChanged(int index)
{
    options.SetButton_OffColor(2, (Manta::LEDState)index);
    thread.ReloadLEDS();
}

void MainWindow::on_button3color_inactive_currentIndexChanged(int index)
{
    options.SetButton_InactiveColor(2, (Manta::LEDState)index);
    thread.ReloadLEDS();
}

void MainWindow::on_button4color_on_currentIndexChanged(int index)
{
    options.SetButton_OnColor(3, (Manta::LEDState)index);
    thread.ReloadLEDS();
}

void MainWindow::on_button4color_off_currentIndexChanged(int index)
{
    options.SetButton_OffColor(3, (Manta::LEDState)index);
    thread.ReloadLEDS();
}

void MainWindow::on_button4color_inactive_currentIndexChanged(int index)
{
    options.SetButton_InactiveColor(3, (Manta::LEDState)index);
    thread.ReloadLEDS();
}

void MainWindow::on_slider1channel_valueChanged(int value)
{
    options.SetSlider_Channel(0, value);
}

void MainWindow::on_slider2channel_valueChanged(int value)
{
    options.SetSlider_Channel(1, value);
}

void MainWindow::on_slider1ccnum_valueChanged(int value)
{
    options.SetSlider_Midi(0, value);
}

void MainWindow::on_slider2ccnum_valueChanged(int value)
{
    options.SetSlider_Midi(1, value);
}

void MainWindow::on_slider1mode_conroller_clicked()
{
    options.SetSlider_Mode(0, smContinuous);
}

void MainWindow::on_slider1mode_pitchbend_clicked()
{
    options.SetSlider_Mode(0, smPitchBend);
}

void MainWindow::on_slider2mode_controller_clicked()
{
    options.SetSlider_Mode(1, smContinuous);
}

void MainWindow::on_slider2mode_pitchbend_clicked()
{
    options.SetSlider_Mode(1, smPitchBend);
}

void MainWindow::on_pushButton_clicked()
{
    if (!padSettingsDialog)
    {
        padSettingsDialog = new PadSettingsDialog(this);
        //connect(padSettingsDialog, SIGNAL(), this, SLOT());
    }

    padSettingsDialog->show();
    padSettingsDialog->raise();
    padSettingsDialog->activateWindow();
}
