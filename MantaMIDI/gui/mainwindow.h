#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "mantathread.h"
#include "QMantaSettings.h"

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

    void on_padAftertouch_Monophonic_clicked();
    void on_padAftertouch_Polyphonic_clicked();
    void on_padAftertouch_PolyphonicController_clicked();

    void on_button1Channel_valueChanged(int );
    void on_button2Channel_valueChanged(int );
    void on_button3Channel_valueChanged(int );
    void on_button4Channel_valueChanged(int );

    void on_button1Note_valueChanged(int );
    void on_button2Note_valueChanged(int );
    void on_button3Note_valueChanged(int );
    void on_button4Note_valueChanged(int );

    void on_button1type_control_clicked();
    void on_button1type_note_clicked();
    void on_button2type_control_clicked();
    void on_button2type_note_clicked();
    void on_button3type_control_clicked();
    void on_button3type_note_clicked();
    void on_button4type_control_clicked();
    void on_button4type_note_clicked();

    void on_button1color_on_currentIndexChanged(int index);
    void on_button1color_off_currentIndexChanged(int index);
    void on_button1color_inactive_currentIndexChanged(int index);
    void on_button2color_on_currentIndexChanged(int index);
    void on_button2color_off_currentIndexChanged(int index);
    void on_button2color_inactive_currentIndexChanged(int index);
    void on_button3color_on_currentIndexChanged(int index);
    void on_button3color_off_currentIndexChanged(int index);
    void on_button3color_inactive_currentIndexChanged(int index);
    void on_button4color_on_currentIndexChanged(int index);
    void on_button4color_off_currentIndexChanged(int index);
    void on_button4color_inactive_currentIndexChanged(int index);

    void on_slider1channel_valueChanged(int );
    void on_slider1ccnum_valueChanged(int );
    void on_slider2channel_valueChanged(int );
    void on_slider2ccnum_valueChanged(int );
    void on_slider1mode_controller_clicked();
    void on_slider1mode_pitchbend_clicked();
    void on_slider2mode_controller_clicked();
    void on_slider2mode_pitchbend_clicked();

    void on_actionSave_Preset_triggered();
    void on_actionLoad_Preset_triggered();
    void on_padSelectionSpin_valueChanged(int );
    void on_padChannelSpin_valueChanged(int );
    void on_padNoteSpin_valueChanged(int );
    void on_padReceiveChannelSpin_valueChanged(int );
    void on_padReceiveAmberNoteSpin_valueChanged(int );
    void on_padReceiveRedNoteSpin_valueChanged(int );
    void on_receiveMirrorsSendCheck_clicked(bool checked);

private:
    void UpdateLayout(PadLayout padLayout);
    void ReloadForms();

    void SetPadMIDIFormFromIndex(int index);
    void SetPadRecieveAmberFormFromIndex(int index);
    void SetPadRecieveRedFormFromIndex(int index);

    Ui::MainWindow *ui;

    QMantaMidiSettings options;
    MantaThread thread;
    bool m_bReceiveMirrorsSend;
};

#endif // MAINWINDOW_H
