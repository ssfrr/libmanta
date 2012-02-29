#ifndef MANTATHREAD_H
#define MANTATHREAD_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>

#include "../settings/MantaMidiSettings.h"
#include "../midimanager/RtMidiManager.h"

enum MantaMode
{
    eMode_Run = 0,
    eMode_Diagnostic = 1,
    eMode_Disabled = 2,
    eMode_Exit = 3
};

class MantaThread : public QThread
{
    Q_OBJECT
public:
    MantaThread(QObject *parent = 0);
    ~MantaThread();

    void Setup(MantaMidiSettings *options);
    void ReloadLEDS();

signals:
    void MantaConnectedMessage(const QString &output);
    void UpdateStatusMessage(const QString &output);
    void SendDebugMessage(const QString &output);

protected:
    void run();

private slots:
    void ForceMantaDisconnect();
    void MantaReconnect();
    void RunMantaDiagnostic();
    void CalibrateMode();
    void RunManta();

private:
    void RunDiagnostic();
    void RunPadDiagnostic();
    void RunButtonDiagnostic();
    void RunSliderDiagnostic();

    void RunCalibration();

    QMutex mutex;
    QWaitCondition condition;
    MantaMode m_runningMode;

    MantaMidiSettings *m_options;
    MidiManager *manta;
};

#endif // MANTATHREAD_H
