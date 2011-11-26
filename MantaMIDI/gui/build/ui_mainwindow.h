/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Fri Oct 21 10:03:38 2011
**      by: Qt User Interface Compiler version 4.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLoad_Preset;
    QAction *actionSave_Preset;
    QAction *actionForce_Disconnect;
    QAction *actionRun_LED_Diagnostic;
    QAction *actionReconnect;
    QAction *actionCalibrate;
    QWidget *centralWidget;
    QGroupBox *groupBox_4;
    QCheckBox *checkBox;
    QGroupBox *groupBox;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_5;
    QSpinBox *button1Channel;
    QRadioButton *button1type_control;
    QRadioButton *button1type_note;
    QComboBox *button1color_on;
    QComboBox *button1color_off;
    QComboBox *button1color_inactive;
    QLabel *label_6;
    QSpinBox *button2Channel;
    QSpinBox *button2Note;
    QRadioButton *button2type_control;
    QRadioButton *button2type_note;
    QComboBox *button2color_on;
    QComboBox *button2color_inactive;
    QLabel *label_10;
    QSpinBox *button3Channel;
    QSpinBox *button3Note;
    QRadioButton *button3type_control;
    QRadioButton *button3type_note;
    QComboBox *button3color_on;
    QComboBox *button3color_off;
    QComboBox *button3color_inactive;
    QLabel *label_11;
    QSpinBox *button4Channel;
    QSpinBox *button4Note;
    QRadioButton *button4type_control;
    QRadioButton *button4type_note;
    QComboBox *button4color_on;
    QComboBox *button4color_off;
    QComboBox *button4color_inactive;
    QComboBox *button2color_off;
    QSpinBox *button1Note;
    QGroupBox *groupBox_2;
    QSpinBox *slider1channel;
    QLabel *label_3;
    QLabel *label_29;
    QLabel *label_30;
    QLabel *label_8;
    QSpinBox *slider1ccnum;
    QLabel *label_28;
    QSpinBox *slider2ccnum;
    QSpinBox *slider2channel;
    QWidget *layoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *slider2mode_controller;
    QRadioButton *slider2mode_pitchbend;
    QWidget *layoutWidget_3;
    QVBoxLayout *verticalLayout;
    QRadioButton *slider1mode_controller;
    QRadioButton *slider1mode_pitchbend;
    QLabel *label_31;
    QLabel *label_4;
    QLabel *label_32;
    QGroupBox *groupBox_3;
    QGroupBox *groupBox_5;
    QRadioButton *layoutRB_HaydenDuet;
    QRadioButton *layoutRB_Honeycomb;
    QRadioButton *layoutRB_Piano;
    QRadioButton *layoutRB_Chromatic;
    QGroupBox *groupBox_8;
    QRadioButton *padAftertouch_Monophonic;
    QRadioButton *padAftertouch_Polyphonic;
    QRadioButton *padAftertouch_PolyphonicController;
    QGroupBox *groupBox_6;
    QLabel *label_9;
    QLabel *label_7;
    QComboBox *padcolor_off;
    QComboBox *padcolor_on;
    QLabel *label_19;
    QSpinBox *padSelectionSpin;
    QLabel *label_20;
    QSpinBox *padSendChannelSpin;
    QSpinBox *padSendNoteSpin;
    QLabel *label;
    QComboBox *padcolor_inactive;
    QLabel *label_21;
    QCheckBox *receiveMirrorsSendCheck;
    QSpinBox *padReceiveAmberNoteSpin;
    QSpinBox *padReceiveChannelSpin;
    QSpinBox *padReceiveRedNoteSpin;
    QLabel *label_22;
    QLabel *label_23;
    QLabel *label_24;
    QLabel *testtext;
    QLabel *mantaConnectedLbl;
    QTextEdit *debugOutput;
    QMenuBar *menuBar;
    QMenu *menuOptions;
    QMenu *menuTesting;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QButtonGroup *button4TypeGroup;
    QButtonGroup *padAftertouchModeGroup;
    QButtonGroup *padLayoutGroup;
    QButtonGroup *button1TypeGroup;
    QButtonGroup *button2TypeGroup;
    QButtonGroup *button3TypeGroup;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(606, 753);
        actionLoad_Preset = new QAction(MainWindow);
        actionLoad_Preset->setObjectName(QString::fromUtf8("actionLoad_Preset"));
        actionSave_Preset = new QAction(MainWindow);
        actionSave_Preset->setObjectName(QString::fromUtf8("actionSave_Preset"));
        actionForce_Disconnect = new QAction(MainWindow);
        actionForce_Disconnect->setObjectName(QString::fromUtf8("actionForce_Disconnect"));
        actionRun_LED_Diagnostic = new QAction(MainWindow);
        actionRun_LED_Diagnostic->setObjectName(QString::fromUtf8("actionRun_LED_Diagnostic"));
        actionReconnect = new QAction(MainWindow);
        actionReconnect->setObjectName(QString::fromUtf8("actionReconnect"));
        actionCalibrate = new QAction(MainWindow);
        actionCalibrate->setObjectName(QString::fromUtf8("actionCalibrate"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 0, 121, 61));
        checkBox = new QCheckBox(groupBox_4);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(10, 30, 111, 20));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 70, 571, 201));
        groupBox->setMaximumSize(QSize(571, 16777215));
        layoutWidget = new QWidget(groupBox);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 30, 550, 161));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        label_12 = new QLabel(layoutWidget);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_12, 0, 1, 1, 1);

        label_13 = new QLabel(layoutWidget);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_13, 0, 2, 1, 1);

        label_14 = new QLabel(layoutWidget);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_14, 0, 3, 1, 2);

        label_15 = new QLabel(layoutWidget);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_15, 0, 5, 1, 1);

        label_16 = new QLabel(layoutWidget);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_16, 0, 6, 1, 1);

        label_17 = new QLabel(layoutWidget);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_17, 0, 7, 1, 1);

        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_5, 1, 0, 1, 1);

        button1Channel = new QSpinBox(layoutWidget);
        button1Channel->setObjectName(QString::fromUtf8("button1Channel"));
        button1Channel->setMinimum(1);
        button1Channel->setMaximum(16);

        gridLayout->addWidget(button1Channel, 1, 1, 1, 1);

        button1type_control = new QRadioButton(layoutWidget);
        button1TypeGroup = new QButtonGroup(MainWindow);
        button1TypeGroup->setObjectName(QString::fromUtf8("button1TypeGroup"));
        button1TypeGroup->addButton(button1type_control);
        button1type_control->setObjectName(QString::fromUtf8("button1type_control"));

        gridLayout->addWidget(button1type_control, 1, 3, 1, 1);

        button1type_note = new QRadioButton(layoutWidget);
        button1TypeGroup->addButton(button1type_note);
        button1type_note->setObjectName(QString::fromUtf8("button1type_note"));

        gridLayout->addWidget(button1type_note, 1, 4, 1, 1);

        button1color_on = new QComboBox(layoutWidget);
        button1color_on->setObjectName(QString::fromUtf8("button1color_on"));

        gridLayout->addWidget(button1color_on, 1, 5, 1, 1);

        button1color_off = new QComboBox(layoutWidget);
        button1color_off->setObjectName(QString::fromUtf8("button1color_off"));

        gridLayout->addWidget(button1color_off, 1, 6, 1, 1);

        button1color_inactive = new QComboBox(layoutWidget);
        button1color_inactive->setObjectName(QString::fromUtf8("button1color_inactive"));

        gridLayout->addWidget(button1color_inactive, 1, 7, 1, 1);

        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_6, 2, 0, 1, 1);

        button2Channel = new QSpinBox(layoutWidget);
        button2Channel->setObjectName(QString::fromUtf8("button2Channel"));
        button2Channel->setMinimum(1);
        button2Channel->setMaximum(16);

        gridLayout->addWidget(button2Channel, 2, 1, 1, 1);

        button2Note = new QSpinBox(layoutWidget);
        button2Note->setObjectName(QString::fromUtf8("button2Note"));
        button2Note->setMinimum(0);
        button2Note->setMaximum(128);
        button2Note->setValue(65);

        gridLayout->addWidget(button2Note, 2, 2, 1, 1);

        button2type_control = new QRadioButton(layoutWidget);
        button2TypeGroup = new QButtonGroup(MainWindow);
        button2TypeGroup->setObjectName(QString::fromUtf8("button2TypeGroup"));
        button2TypeGroup->addButton(button2type_control);
        button2type_control->setObjectName(QString::fromUtf8("button2type_control"));

        gridLayout->addWidget(button2type_control, 2, 3, 1, 1);

        button2type_note = new QRadioButton(layoutWidget);
        button2TypeGroup->addButton(button2type_note);
        button2type_note->setObjectName(QString::fromUtf8("button2type_note"));

        gridLayout->addWidget(button2type_note, 2, 4, 1, 1);

        button2color_on = new QComboBox(layoutWidget);
        button2color_on->setObjectName(QString::fromUtf8("button2color_on"));

        gridLayout->addWidget(button2color_on, 2, 5, 1, 1);

        button2color_inactive = new QComboBox(layoutWidget);
        button2color_inactive->setObjectName(QString::fromUtf8("button2color_inactive"));

        gridLayout->addWidget(button2color_inactive, 2, 7, 1, 1);

        label_10 = new QLabel(layoutWidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_10, 3, 0, 1, 1);

        button3Channel = new QSpinBox(layoutWidget);
        button3Channel->setObjectName(QString::fromUtf8("button3Channel"));
        button3Channel->setMinimum(1);
        button3Channel->setMaximum(16);

        gridLayout->addWidget(button3Channel, 3, 1, 1, 1);

        button3Note = new QSpinBox(layoutWidget);
        button3Note->setObjectName(QString::fromUtf8("button3Note"));
        button3Note->setMinimum(0);
        button3Note->setMaximum(128);
        button3Note->setValue(66);

        gridLayout->addWidget(button3Note, 3, 2, 1, 1);

        button3type_control = new QRadioButton(layoutWidget);
        button3TypeGroup = new QButtonGroup(MainWindow);
        button3TypeGroup->setObjectName(QString::fromUtf8("button3TypeGroup"));
        button3TypeGroup->addButton(button3type_control);
        button3type_control->setObjectName(QString::fromUtf8("button3type_control"));

        gridLayout->addWidget(button3type_control, 3, 3, 1, 1);

        button3type_note = new QRadioButton(layoutWidget);
        button3TypeGroup->addButton(button3type_note);
        button3type_note->setObjectName(QString::fromUtf8("button3type_note"));

        gridLayout->addWidget(button3type_note, 3, 4, 1, 1);

        button3color_on = new QComboBox(layoutWidget);
        button3color_on->setObjectName(QString::fromUtf8("button3color_on"));

        gridLayout->addWidget(button3color_on, 3, 5, 1, 1);

        button3color_off = new QComboBox(layoutWidget);
        button3color_off->setObjectName(QString::fromUtf8("button3color_off"));

        gridLayout->addWidget(button3color_off, 3, 6, 1, 1);

        button3color_inactive = new QComboBox(layoutWidget);
        button3color_inactive->setObjectName(QString::fromUtf8("button3color_inactive"));

        gridLayout->addWidget(button3color_inactive, 3, 7, 1, 1);

        label_11 = new QLabel(layoutWidget);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_11, 4, 0, 1, 1);

        button4Channel = new QSpinBox(layoutWidget);
        button4Channel->setObjectName(QString::fromUtf8("button4Channel"));
        button4Channel->setMinimum(1);
        button4Channel->setMaximum(16);

        gridLayout->addWidget(button4Channel, 4, 1, 1, 1);

        button4Note = new QSpinBox(layoutWidget);
        button4Note->setObjectName(QString::fromUtf8("button4Note"));
        button4Note->setMinimum(0);
        button4Note->setMaximum(128);
        button4Note->setValue(67);

        gridLayout->addWidget(button4Note, 4, 2, 1, 1);

        button4type_control = new QRadioButton(layoutWidget);
        button4TypeGroup = new QButtonGroup(MainWindow);
        button4TypeGroup->setObjectName(QString::fromUtf8("button4TypeGroup"));
        button4TypeGroup->addButton(button4type_control);
        button4type_control->setObjectName(QString::fromUtf8("button4type_control"));

        gridLayout->addWidget(button4type_control, 4, 3, 1, 1);

        button4type_note = new QRadioButton(layoutWidget);
        button4TypeGroup->addButton(button4type_note);
        button4type_note->setObjectName(QString::fromUtf8("button4type_note"));

        gridLayout->addWidget(button4type_note, 4, 4, 1, 1);

        button4color_on = new QComboBox(layoutWidget);
        button4color_on->setObjectName(QString::fromUtf8("button4color_on"));

        gridLayout->addWidget(button4color_on, 4, 5, 1, 1);

        button4color_off = new QComboBox(layoutWidget);
        button4color_off->setObjectName(QString::fromUtf8("button4color_off"));

        gridLayout->addWidget(button4color_off, 4, 6, 1, 1);

        button4color_inactive = new QComboBox(layoutWidget);
        button4color_inactive->setObjectName(QString::fromUtf8("button4color_inactive"));

        gridLayout->addWidget(button4color_inactive, 4, 7, 1, 1);

        button2color_off = new QComboBox(layoutWidget);
        button2color_off->setObjectName(QString::fromUtf8("button2color_off"));

        gridLayout->addWidget(button2color_off, 2, 6, 1, 1);

        button1Note = new QSpinBox(layoutWidget);
        button1Note->setObjectName(QString::fromUtf8("button1Note"));
        button1Note->setMinimum(0);
        button1Note->setMaximum(128);
        button1Note->setValue(64);

        gridLayout->addWidget(button1Note, 1, 2, 1, 1);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 270, 571, 111));
        slider1channel = new QSpinBox(groupBox_2);
        slider1channel->setObjectName(QString::fromUtf8("slider1channel"));
        slider1channel->setGeometry(QRect(39, 67, 45, 25));
        slider1channel->setMinimum(1);
        slider1channel->setMaximum(16);
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(30, 32, 52, 16));
        label_3->setAlignment(Qt::AlignCenter);
        label_29 = new QLabel(groupBox_2);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setGeometry(QRect(100, 32, 30, 16));
        label_29->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_30 = new QLabel(groupBox_2);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setGeometry(QRect(183, 32, 35, 16));
        label_30->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 70, 16, 16));
        label_8->setAlignment(Qt::AlignCenter);
        slider1ccnum = new QSpinBox(groupBox_2);
        slider1ccnum->setObjectName(QString::fromUtf8("slider1ccnum"));
        slider1ccnum->setGeometry(QRect(90, 67, 53, 25));
        slider1ccnum->setMinimum(0);
        slider1ccnum->setMaximum(128);
        label_28 = new QLabel(groupBox_2);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setGeometry(QRect(313, 73, 16, 16));
        label_28->setAlignment(Qt::AlignCenter);
        slider2ccnum = new QSpinBox(groupBox_2);
        slider2ccnum->setObjectName(QString::fromUtf8("slider2ccnum"));
        slider2ccnum->setGeometry(QRect(383, 69, 53, 25));
        slider2ccnum->setMinimum(0);
        slider2ccnum->setMaximum(128);
        slider2channel = new QSpinBox(groupBox_2);
        slider2channel->setObjectName(QString::fromUtf8("slider2channel"));
        slider2channel->setGeometry(QRect(332, 69, 45, 25));
        slider2channel->setMinimum(1);
        slider2channel->setMaximum(16);
        layoutWidget_2 = new QWidget(groupBox_2);
        layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(443, 60, 99, 41));
        verticalLayout_2 = new QVBoxLayout(layoutWidget_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        slider2mode_controller = new QRadioButton(layoutWidget_2);
        slider2mode_controller->setObjectName(QString::fromUtf8("slider2mode_controller"));

        verticalLayout_2->addWidget(slider2mode_controller);

        slider2mode_pitchbend = new QRadioButton(layoutWidget_2);
        slider2mode_pitchbend->setObjectName(QString::fromUtf8("slider2mode_pitchbend"));

        verticalLayout_2->addWidget(slider2mode_pitchbend);

        layoutWidget_3 = new QWidget(groupBox_2);
        layoutWidget_3->setObjectName(QString::fromUtf8("layoutWidget_3"));
        layoutWidget_3->setGeometry(QRect(150, 60, 99, 39));
        verticalLayout = new QVBoxLayout(layoutWidget_3);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        slider1mode_controller = new QRadioButton(layoutWidget_3);
        slider1mode_controller->setObjectName(QString::fromUtf8("slider1mode_controller"));

        verticalLayout->addWidget(slider1mode_controller);

        slider1mode_pitchbend = new QRadioButton(layoutWidget_3);
        slider1mode_pitchbend->setObjectName(QString::fromUtf8("slider1mode_pitchbend"));

        verticalLayout->addWidget(slider1mode_pitchbend);

        label_31 = new QLabel(groupBox_2);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setGeometry(QRect(483, 30, 35, 16));
        label_31->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(330, 30, 52, 16));
        label_4->setAlignment(Qt::AlignCenter);
        label_32 = new QLabel(groupBox_2);
        label_32->setObjectName(QString::fromUtf8("label_32"));
        label_32->setGeometry(QRect(400, 30, 30, 16));
        label_32->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 380, 590, 480));
        groupBox_5 = new QGroupBox(groupBox_3);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setGeometry(QRect(10, 20, 161, 110));
        layoutRB_HaydenDuet = new QRadioButton(groupBox_5);
        padLayoutGroup = new QButtonGroup(MainWindow);
        padLayoutGroup->setObjectName(QString::fromUtf8("padLayoutGroup"));
        padLayoutGroup->addButton(layoutRB_HaydenDuet);
        layoutRB_HaydenDuet->setObjectName(QString::fromUtf8("layoutRB_HaydenDuet"));
        layoutRB_HaydenDuet->setGeometry(QRect(10, 80, 102, 20));
        layoutRB_Honeycomb = new QRadioButton(groupBox_5);
        padLayoutGroup->addButton(layoutRB_Honeycomb);
        layoutRB_Honeycomb->setObjectName(QString::fromUtf8("layoutRB_Honeycomb"));
        layoutRB_Honeycomb->setGeometry(QRect(10, 20, 102, 20));
        layoutRB_Honeycomb->setChecked(true);
        layoutRB_Piano = new QRadioButton(groupBox_5);
        padLayoutGroup->addButton(layoutRB_Piano);
        layoutRB_Piano->setObjectName(QString::fromUtf8("layoutRB_Piano"));
        layoutRB_Piano->setGeometry(QRect(10, 40, 102, 20));
        layoutRB_Chromatic = new QRadioButton(groupBox_5);
        padLayoutGroup->addButton(layoutRB_Chromatic);
        layoutRB_Chromatic->setObjectName(QString::fromUtf8("layoutRB_Chromatic"));
        layoutRB_Chromatic->setGeometry(QRect(10, 60, 102, 20));
        groupBox_8 = new QGroupBox(groupBox_3);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        groupBox_8->setGeometry(QRect(180, 20, 191, 90));
        padAftertouch_Monophonic = new QRadioButton(groupBox_8);
        padAftertouchModeGroup = new QButtonGroup(MainWindow);
        padAftertouchModeGroup->setObjectName(QString::fromUtf8("padAftertouchModeGroup"));
        padAftertouchModeGroup->addButton(padAftertouch_Monophonic);
        padAftertouch_Monophonic->setObjectName(QString::fromUtf8("padAftertouch_Monophonic"));
        padAftertouch_Monophonic->setGeometry(QRect(10, 20, 171, 20));
        padAftertouch_Polyphonic = new QRadioButton(groupBox_8);
        padAftertouchModeGroup->addButton(padAftertouch_Polyphonic);
        padAftertouch_Polyphonic->setObjectName(QString::fromUtf8("padAftertouch_Polyphonic"));
        padAftertouch_Polyphonic->setGeometry(QRect(10, 40, 171, 20));
        padAftertouch_PolyphonicController = new QRadioButton(groupBox_8);
        padAftertouchModeGroup->addButton(padAftertouch_PolyphonicController);
        padAftertouch_PolyphonicController->setObjectName(QString::fromUtf8("padAftertouch_PolyphonicController"));
        padAftertouch_PolyphonicController->setGeometry(QRect(10, 60, 171, 20));
        groupBox_6 = new QGroupBox(groupBox_3);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        groupBox_6->setGeometry(QRect(10, 130, 570, 180));
        label_9 = new QLabel(groupBox_6);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(510, 60, 62, 16));
        label_7 = new QLabel(groupBox_6);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(370, 60, 62, 16));
        padcolor_off = new QComboBox(groupBox_6);
        padcolor_off->setObjectName(QString::fromUtf8("padcolor_off"));
        padcolor_off->setGeometry(QRect(424, 80, 70, 26));
        padcolor_on = new QComboBox(groupBox_6);
        padcolor_on->setObjectName(QString::fromUtf8("padcolor_on"));
        padcolor_on->setGeometry(QRect(348, 80, 70, 26));
        label_19 = new QLabel(groupBox_6);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(70, 60, 50, 16));
        padSelectionSpin = new QSpinBox(groupBox_6);
        padSelectionSpin->setObjectName(QString::fromUtf8("padSelectionSpin"));
        padSelectionSpin->setGeometry(QRect(40, 28, 50, 20));
        padSelectionSpin->setMinimum(1);
        padSelectionSpin->setMaximum(48);
        label_20 = new QLabel(groupBox_6);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(440, 60, 62, 16));
        padSendChannelSpin = new QSpinBox(groupBox_6);
        padSendChannelSpin->setObjectName(QString::fromUtf8("padSendChannelSpin"));
        padSendChannelSpin->setGeometry(QRect(70, 80, 57, 25));
        padSendChannelSpin->setMinimum(1);
        padSendChannelSpin->setMaximum(16);
        padSendNoteSpin = new QSpinBox(groupBox_6);
        padSendNoteSpin->setObjectName(QString::fromUtf8("padSendNoteSpin"));
        padSendNoteSpin->setGeometry(QRect(140, 80, 57, 25));
        padSendNoteSpin->setMinimum(0);
        padSendNoteSpin->setMaximum(128);
        padSendNoteSpin->setValue(64);
        label = new QLabel(groupBox_6);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 30, 30, 16));
        padcolor_inactive = new QComboBox(groupBox_6);
        padcolor_inactive->setObjectName(QString::fromUtf8("padcolor_inactive"));
        padcolor_inactive->setGeometry(QRect(500, 80, 70, 26));
        label_21 = new QLabel(groupBox_6);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(150, 60, 51, 16));
        receiveMirrorsSendCheck = new QCheckBox(groupBox_6);
        receiveMirrorsSendCheck->setObjectName(QString::fromUtf8("receiveMirrorsSendCheck"));
        receiveMirrorsSendCheck->setGeometry(QRect(410, 20, 150, 30));
        padReceiveAmberNoteSpin = new QSpinBox(groupBox_6);
        padReceiveAmberNoteSpin->setObjectName(QString::fromUtf8("padReceiveAmberNoteSpin"));
        padReceiveAmberNoteSpin->setGeometry(QRect(140, 110, 57, 25));
        padReceiveAmberNoteSpin->setMinimum(0);
        padReceiveAmberNoteSpin->setMaximum(128);
        padReceiveAmberNoteSpin->setValue(64);
        padReceiveChannelSpin = new QSpinBox(groupBox_6);
        padReceiveChannelSpin->setObjectName(QString::fromUtf8("padReceiveChannelSpin"));
        padReceiveChannelSpin->setGeometry(QRect(70, 124, 57, 25));
        padReceiveChannelSpin->setMinimum(1);
        padReceiveChannelSpin->setMaximum(16);
        padReceiveRedNoteSpin = new QSpinBox(groupBox_6);
        padReceiveRedNoteSpin->setObjectName(QString::fromUtf8("padReceiveRedNoteSpin"));
        padReceiveRedNoteSpin->setGeometry(QRect(140, 140, 57, 25));
        padReceiveRedNoteSpin->setMinimum(0);
        padReceiveRedNoteSpin->setMaximum(128);
        padReceiveRedNoteSpin->setValue(64);
        label_22 = new QLabel(groupBox_6);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setGeometry(QRect(20, 86, 40, 16));
        label_23 = new QLabel(groupBox_6);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setGeometry(QRect(10, 108, 51, 31));
        label_23->setAlignment(Qt::AlignCenter);
        label_23->setWordWrap(true);
        label_24 = new QLabel(groupBox_6);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setGeometry(QRect(10, 140, 51, 30));
        label_24->setAlignment(Qt::AlignCenter);
        label_24->setWordWrap(true);
        testtext = new QLabel(groupBox_6);
        testtext->setObjectName(QString::fromUtf8("testtext"));
        testtext->setGeometry(QRect(210, 110, 351, 51));
        testtext->setWordWrap(true);
        mantaConnectedLbl = new QLabel(centralWidget);
        mantaConnectedLbl->setObjectName(QString::fromUtf8("mantaConnectedLbl"));
        mantaConnectedLbl->setGeometry(QRect(420, 30, 151, 20));
        mantaConnectedLbl->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        debugOutput = new QTextEdit(centralWidget);
        debugOutput->setObjectName(QString::fromUtf8("debugOutput"));
        debugOutput->setGeometry(QRect(180, 0, 231, 78));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 606, 22));
        menuOptions = new QMenu(menuBar);
        menuOptions->setObjectName(QString::fromUtf8("menuOptions"));
        menuTesting = new QMenu(menuBar);
        menuTesting->setObjectName(QString::fromUtf8("menuTesting"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuOptions->menuAction());
        menuBar->addAction(menuTesting->menuAction());
        menuOptions->addAction(actionLoad_Preset);
        menuOptions->addAction(actionSave_Preset);
        menuTesting->addAction(actionForce_Disconnect);
        menuTesting->addAction(actionReconnect);
        menuTesting->addAction(actionRun_LED_Diagnostic);
        menuTesting->addAction(actionCalibrate);

        retranslateUi(MainWindow);
        QObject::connect(actionReconnect, SIGNAL(triggered()), actionReconnect, SLOT(trigger()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionLoad_Preset->setText(QApplication::translate("MainWindow", "Load Preset", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionLoad_Preset->setToolTip(QApplication::translate("MainWindow", "Load Preset from File", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionLoad_Preset->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionSave_Preset->setText(QApplication::translate("MainWindow", "Save Preset", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionSave_Preset->setToolTip(QApplication::translate("MainWindow", "Save Preset to File", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionSave_Preset->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionForce_Disconnect->setText(QApplication::translate("MainWindow", "Force Disconnect", 0, QApplication::UnicodeUTF8));
        actionRun_LED_Diagnostic->setText(QApplication::translate("MainWindow", "Run LED Diagnostic", 0, QApplication::UnicodeUTF8));
        actionReconnect->setText(QApplication::translate("MainWindow", "Reconnect", 0, QApplication::UnicodeUTF8));
        actionCalibrate->setText(QApplication::translate("MainWindow", "Calibrate", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "Master Controls", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("MainWindow", "Use Velocity", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "Button Controls", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Button", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("MainWindow", "Channel", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("MainWindow", "Note", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("MainWindow", "Type", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("MainWindow", "On", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("MainWindow", "Inactive", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "1", 0, QApplication::UnicodeUTF8));
        button1type_control->setText(QApplication::translate("MainWindow", "Control", 0, QApplication::UnicodeUTF8));
        button1type_note->setText(QApplication::translate("MainWindow", "Note", 0, QApplication::UnicodeUTF8));
        button1color_on->clear();
        button1color_on->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Amber", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Red", 0, QApplication::UnicodeUTF8)
        );
        button1color_off->clear();
        button1color_off->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Amber", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Red", 0, QApplication::UnicodeUTF8)
        );
        button1color_inactive->clear();
        button1color_inactive->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Amber", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Red", 0, QApplication::UnicodeUTF8)
        );
        label_6->setText(QApplication::translate("MainWindow", "2", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        button2Note->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        button2type_control->setText(QApplication::translate("MainWindow", "Control", 0, QApplication::UnicodeUTF8));
        button2type_note->setText(QApplication::translate("MainWindow", "Note", 0, QApplication::UnicodeUTF8));
        button2color_on->clear();
        button2color_on->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Amber", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Red", 0, QApplication::UnicodeUTF8)
        );
        button2color_inactive->clear();
        button2color_inactive->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Amber", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Red", 0, QApplication::UnicodeUTF8)
        );
        label_10->setText(QApplication::translate("MainWindow", "3", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        button3Note->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        button3type_control->setText(QApplication::translate("MainWindow", "Control", 0, QApplication::UnicodeUTF8));
        button3type_note->setText(QApplication::translate("MainWindow", "Note", 0, QApplication::UnicodeUTF8));
        button3color_on->clear();
        button3color_on->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Amber", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Red", 0, QApplication::UnicodeUTF8)
        );
        button3color_off->clear();
        button3color_off->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Amber", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Red", 0, QApplication::UnicodeUTF8)
        );
        button3color_inactive->clear();
        button3color_inactive->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Amber", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Red", 0, QApplication::UnicodeUTF8)
        );
        label_11->setText(QApplication::translate("MainWindow", "4", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        button4Note->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        button4type_control->setText(QApplication::translate("MainWindow", "Control", 0, QApplication::UnicodeUTF8));
        button4type_note->setText(QApplication::translate("MainWindow", "Note", 0, QApplication::UnicodeUTF8));
        button4color_on->clear();
        button4color_on->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Amber", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Red", 0, QApplication::UnicodeUTF8)
        );
        button4color_off->clear();
        button4color_off->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Amber", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Red", 0, QApplication::UnicodeUTF8)
        );
        button4color_inactive->clear();
        button4color_inactive->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Amber", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Red", 0, QApplication::UnicodeUTF8)
        );
        button2color_off->clear();
        button2color_off->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Amber", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Red", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        button1Note->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Slider Controls", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Channel", 0, QApplication::UnicodeUTF8));
        label_29->setText(QApplication::translate("MainWindow", "Note", 0, QApplication::UnicodeUTF8));
        label_30->setText(QApplication::translate("MainWindow", "Mode", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "1", 0, QApplication::UnicodeUTF8));
        label_28->setText(QApplication::translate("MainWindow", "2", 0, QApplication::UnicodeUTF8));
        slider2mode_controller->setText(QApplication::translate("MainWindow", "Continuous", 0, QApplication::UnicodeUTF8));
        slider2mode_pitchbend->setText(QApplication::translate("MainWindow", "Pitchbend", 0, QApplication::UnicodeUTF8));
        slider1mode_controller->setText(QApplication::translate("MainWindow", "Continuous", 0, QApplication::UnicodeUTF8));
        slider1mode_pitchbend->setText(QApplication::translate("MainWindow", "Pitchbend", 0, QApplication::UnicodeUTF8));
        label_31->setText(QApplication::translate("MainWindow", "Mode", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Channel", 0, QApplication::UnicodeUTF8));
        label_32->setText(QApplication::translate("MainWindow", "Note", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Pad Controls", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "Layout", 0, QApplication::UnicodeUTF8));
        layoutRB_HaydenDuet->setText(QApplication::translate("MainWindow", "Hayden Duet", 0, QApplication::UnicodeUTF8));
        layoutRB_Honeycomb->setText(QApplication::translate("MainWindow", "Honeycomb", 0, QApplication::UnicodeUTF8));
        layoutRB_Piano->setText(QApplication::translate("MainWindow", "Piano", 0, QApplication::UnicodeUTF8));
        layoutRB_Chromatic->setText(QApplication::translate("MainWindow", "Chromatic", 0, QApplication::UnicodeUTF8));
        groupBox_8->setTitle(QApplication::translate("MainWindow", "Aftertouch Mode", 0, QApplication::UnicodeUTF8));
        padAftertouch_Monophonic->setText(QApplication::translate("MainWindow", "Monophonic Aftertouch", 0, QApplication::UnicodeUTF8));
        padAftertouch_Polyphonic->setText(QApplication::translate("MainWindow", "Polyphonic Aftertouch", 0, QApplication::UnicodeUTF8));
        padAftertouch_PolyphonicController->setText(QApplication::translate("MainWindow", "Polyphonic Controllers", 0, QApplication::UnicodeUTF8));
        groupBox_6->setTitle(QApplication::translate("MainWindow", "Pad MIDI", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("MainWindow", "Inactive", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "On", 0, QApplication::UnicodeUTF8));
        padcolor_off->clear();
        padcolor_off->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Amber", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Red", 0, QApplication::UnicodeUTF8)
        );
        padcolor_on->clear();
        padcolor_on->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Amber", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Red", 0, QApplication::UnicodeUTF8)
        );
        label_19->setText(QApplication::translate("MainWindow", "Channel", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        padSendNoteSpin->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        label->setText(QApplication::translate("MainWindow", "Pad", 0, QApplication::UnicodeUTF8));
        padcolor_inactive->clear();
        padcolor_inactive->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Amber", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Red", 0, QApplication::UnicodeUTF8)
        );
        label_21->setText(QApplication::translate("MainWindow", "Note", 0, QApplication::UnicodeUTF8));
        receiveMirrorsSendCheck->setText(QApplication::translate("MainWindow", "MIDI Receive Mirrors Send", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        padReceiveAmberNoteSpin->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        padReceiveRedNoteSpin->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        label_22->setText(QApplication::translate("MainWindow", "Send", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("MainWindow", "Receive Amber", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("MainWindow", "Receive Red", 0, QApplication::UnicodeUTF8));
        testtext->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
        mantaConnectedLbl->setText(QApplication::translate("MainWindow", "Manta Not Connected", 0, QApplication::UnicodeUTF8));
        menuOptions->setTitle(QApplication::translate("MainWindow", "Options", 0, QApplication::UnicodeUTF8));
        menuTesting->setTitle(QApplication::translate("MainWindow", "Testing", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
