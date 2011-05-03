#include "mantathread.h"

MantaThread::MantaThread(QObject *parent) :
    QThread(parent), m_options(NULL), manta(NULL)
{
}

MantaThread::~MantaThread()
{
    mutex.lock();
    bExit = true;
    manta->Disconnect();
    condition.wait(&mutex);
    mutex.unlock();

    delete manta;
}

void MantaThread::Setup(OptionHolder *options)
{
    m_options = options;
    manta = new RtMidiManager(options);
}

void MantaThread::ReloadLEDS()
{
    manta->ResetLEDS();
}

void MantaThread::run()
{
    forever
    {
        if (manta && m_options)
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

                    if (bExit)
                        break;
                }
            }
            catch(MantaCommunicationException &e)
            {
                emit UpdateStatusMessage("Communication with Manta interrrupted...");
                emit MantaConnectedMessage("Manta Not Connected");
            }
        }
        else
            emit UpdateStatusMessage("Manta Object Not initialized!!!");

        if (bExit)
            break;
    }
}
