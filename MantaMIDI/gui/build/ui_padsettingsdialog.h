/********************************************************************************
** Form generated from reading UI file 'padsettingsdialog.ui'
**
** Created: Wed Jul 20 23:04:09 2011
**      by: Qt User Interface Compiler version 4.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PADSETTINGSDIALOG_H
#define UI_PADSETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGraphicsView>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_PadSettingsDialog
{
public:
    QDialogButtonBox *buttonBox;
    QGraphicsView *padLayoutView;
    QSpinBox *padSelectionSpin;
    QLabel *label;
    QComboBox *padcolor_on;
    QSpinBox *padNoteSpin;
    QSpinBox *padChannelSpin;
    QRadioButton *padMode_note;
    QRadioButton *padMode_control;
    QComboBox *padcolor_off;
    QComboBox *padcolor_inactive;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *debugLbl;

    void setupUi(QDialog *PadSettingsDialog)
    {
        if (PadSettingsDialog->objectName().isEmpty())
            PadSettingsDialog->setObjectName(QString::fromUtf8("PadSettingsDialog"));
        PadSettingsDialog->resize(637, 387);
        buttonBox = new QDialogButtonBox(PadSettingsDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(280, 330, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        padLayoutView = new QGraphicsView(PadSettingsDialog);
        padLayoutView->setObjectName(QString::fromUtf8("padLayoutView"));
        padLayoutView->setGeometry(QRect(40, 210, 141, 111));
        padSelectionSpin = new QSpinBox(PadSettingsDialog);
        padSelectionSpin->setObjectName(QString::fromUtf8("padSelectionSpin"));
        padSelectionSpin->setGeometry(QRect(40, 80, 57, 25));
        padSelectionSpin->setMinimum(1);
        padSelectionSpin->setMaximum(48);
        label = new QLabel(PadSettingsDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 50, 91, 16));
        padcolor_on = new QComboBox(PadSettingsDialog);
        padcolor_on->setObjectName(QString::fromUtf8("padcolor_on"));
        padcolor_on->setGeometry(QRect(398, 80, 72, 26));
        padNoteSpin = new QSpinBox(PadSettingsDialog);
        padNoteSpin->setObjectName(QString::fromUtf8("padNoteSpin"));
        padNoteSpin->setGeometry(QRect(191, 80, 57, 25));
        padNoteSpin->setMinimum(0);
        padNoteSpin->setMaximum(128);
        padNoteSpin->setValue(64);
        padChannelSpin = new QSpinBox(PadSettingsDialog);
        padChannelSpin->setObjectName(QString::fromUtf8("padChannelSpin"));
        padChannelSpin->setGeometry(QRect(120, 80, 57, 25));
        padChannelSpin->setMinimum(1);
        padChannelSpin->setMaximum(16);
        padMode_note = new QRadioButton(PadSettingsDialog);
        padMode_note->setObjectName(QString::fromUtf8("padMode_note"));
        padMode_note->setGeometry(QRect(336, 83, 55, 20));
        padMode_control = new QRadioButton(PadSettingsDialog);
        padMode_control->setObjectName(QString::fromUtf8("padMode_control"));
        padMode_control->setGeometry(QRect(258, 83, 68, 20));
        padcolor_off = new QComboBox(PadSettingsDialog);
        padcolor_off->setObjectName(QString::fromUtf8("padcolor_off"));
        padcolor_off->setGeometry(QRect(474, 80, 72, 26));
        padcolor_inactive = new QComboBox(PadSettingsDialog);
        padcolor_inactive->setObjectName(QString::fromUtf8("padcolor_inactive"));
        padcolor_inactive->setGeometry(QRect(550, 80, 72, 26));
        label_2 = new QLabel(PadSettingsDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(120, 50, 62, 16));
        label_3 = new QLabel(PadSettingsDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(190, 50, 51, 16));
        label_4 = new QLabel(PadSettingsDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(290, 50, 62, 16));
        label_5 = new QLabel(PadSettingsDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(410, 50, 62, 16));
        label_7 = new QLabel(PadSettingsDialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(480, 50, 62, 16));
        label_8 = new QLabel(PadSettingsDialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(560, 50, 62, 16));
        debugLbl = new QLabel(PadSettingsDialog);
        debugLbl->setObjectName(QString::fromUtf8("debugLbl"));
        debugLbl->setGeometry(QRect(340, 210, 231, 16));

        retranslateUi(PadSettingsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), PadSettingsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), PadSettingsDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(PadSettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *PadSettingsDialog)
    {
        PadSettingsDialog->setWindowTitle(QApplication::translate("PadSettingsDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("PadSettingsDialog", "Pad Selection", 0, QApplication::UnicodeUTF8));
        padcolor_on->clear();
        padcolor_on->insertItems(0, QStringList()
         << QApplication::translate("PadSettingsDialog", "Off", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PadSettingsDialog", "Amber", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PadSettingsDialog", "Red", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        padNoteSpin->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        padMode_note->setText(QApplication::translate("PadSettingsDialog", "Note", 0, QApplication::UnicodeUTF8));
        padMode_control->setText(QApplication::translate("PadSettingsDialog", "Control", 0, QApplication::UnicodeUTF8));
        padcolor_off->clear();
        padcolor_off->insertItems(0, QStringList()
         << QApplication::translate("PadSettingsDialog", "Off", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PadSettingsDialog", "Amber", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PadSettingsDialog", "Red", 0, QApplication::UnicodeUTF8)
        );
        padcolor_inactive->clear();
        padcolor_inactive->insertItems(0, QStringList()
         << QApplication::translate("PadSettingsDialog", "Off", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PadSettingsDialog", "Amber", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PadSettingsDialog", "Red", 0, QApplication::UnicodeUTF8)
        );
        label_2->setText(QApplication::translate("PadSettingsDialog", "Channel", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("PadSettingsDialog", "Note", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("PadSettingsDialog", "Mode", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("PadSettingsDialog", "On", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("PadSettingsDialog", "Off", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("PadSettingsDialog", "Inactive", 0, QApplication::UnicodeUTF8));
        debugLbl->setText(QApplication::translate("PadSettingsDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PadSettingsDialog: public Ui_PadSettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PADSETTINGSDIALOG_H
