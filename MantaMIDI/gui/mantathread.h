#ifndef MANTATHREAD_H
#define MANTATHREAD_H

#include <QMutex>
#include <QThread>

#include "../OptionHolder.h"
#include "../midimanager/RtMidiManager.h"

class MantaThread : public QThread
{
    Q_OBJECT
public:
    MantaThread(QObject *parent = 0);
    ~MantaThread();

signals:
    void MantaMessage(const QString &output);

protected:
    void run();

private:
    QMutex mutex;

    OptionHolder options;
    MidiManager *manta;


};

#endif // MANTATHREAD_H
