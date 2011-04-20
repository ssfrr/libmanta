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
    forever
    {
        while (!manta->IsConnected())
        {
            try
            {
                manta->Connect();
            }
            catch(MantaNotFoundException &e)
            {
                emit UpdateStatusMessage("Could not find an attached Manta. Retrying...");
                sleep(1);
            }
        }

        emit MantaConnectedMessage("Manta Connected");

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
            emit UpdateStatusMessage("Communication with Manta interrrupted...");
            emit MantaConnectedMessage("Manta Not Connected");
        }
    }
}
