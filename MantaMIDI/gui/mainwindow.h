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

    void on_button1Channel_valueChanged(int );
    void on_button2Channel_valueChanged(int );
    void on_button3Channel_valueChanged(int );
    void on_button4Channel_valueChanged(int );

    void on_button1Note_valueChanged(int );
    void on_button2Note_valueChanged(int );
    void on_button3Note_valueChanged(int );
    void on_button4Note_valueChanged(int );

    void on_button1type_conrol_clicked();
    void on_button1type_note_clicked();
    void on_button2type_control_clicked();
    void on_button2type_note_clicked();
    void on_button3type_control_clicked();
    void on_button3type_note_clicked();
    void on_butotn4type_control_clicked();
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

private:
    void UpdateLayout(PadLayout padLayout);

    Ui::MainWindow *ui;

    OptionHolder options;
    MantaThread thread;
};

#endif // MAINWINDOW_H
