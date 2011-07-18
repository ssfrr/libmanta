#ifndef MANTATHREAD_H
#define MANTATHREAD_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>

#include "../MantaMidiSettings.h"
#include "../midimanager/RtMidiManager.h"

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

protected:
    void run();

private:
    QMutex mutex;
    QWaitCondition condition;
    bool bExit;

    MantaMidiSettings *m_options;
    MidiManager *manta;
};

#endif // MANTATHREAD_H
