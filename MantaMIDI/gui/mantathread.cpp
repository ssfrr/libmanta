#include "mantathread.h"

MantaThread::MantaThread(QObject *parent) :
    QThread(parent)
{
    manta = new RtMidiManager(options);
}

MantaThread::~MantaThread()
{
    delete manta;
}

void MantaThread::run()
{
    do
    {
        try
        {
            manta->Connect();
        }
        catch(MantaNotFoundException &e)
        {
            emit MantaMessage("Could not find an attached Manta. Retrying...");
            sleep(1);
        }
    } while (! manta->IsConnected());

    emit MantaMessage("Manta Connected!");

    try
    {
        manta->Initialize();
        forever
        {
            manta->HandleEvents();
        }
    }
    catch(MantaCommunicationException &e)
    {
        emit MantaMessage("Communication with Manta interrrupted...");
    }

    /*
    int i = 0;
    char buf[128];
    sprintf(buf, "Test %d.", i++);
    emit MantaMessage(QString(buf));
    sleep(2);*/
}
