#include "mantathread.h"

static void waitForTransmitComplete(MantaUSB &manta)
{
   while(manta.IsTransmitting())
   {
      manta.HandleEvents();
   }
}

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

void MantaThread::Setup(MantaMidiSettings *options)
{
    m_options = options;
    manta = new RtMidiManager(options);
}

void MantaThread::ReloadLEDS()
{
    manta->ResetLEDS();
}

void MantaThread::ForceDisconnectManta()
{
    bExit = true;

    if (manta->IsConnected())
    {
        waitForTransmitComplete(*manta);
        manta->Disconnect();

        emit MantaConnectedMessage("Manta Force Disconnected");
    }

    if (this->isRunning())
        this->exit();
}

bool MantaThread::CheckMantaConnected()
{
    while (!manta->IsConnected())
    {
        try
        {
            manta->Connect();

            emit MantaConnectedMessage("Manta Connected");
        }
        catch(MantaNotFoundException &e)
        {
            emit UpdateStatusMessage("Could not find an attached Manta. Retrying...");
            sleep(1);
        }
    }

    return manta->IsConnected();
}

void MantaThread::RunDiagnostic()
{
    if (this->isRunning())
        this->exit();

    if (manta && CheckMantaConnected())
    {
        manta->SetLEDControl(Manta::PadAndButton, true);
        waitForTransmitComplete(*manta);
        manta->SetLEDControl(Manta::Slider, true);
        waitForTransmitComplete(*manta);

        RunPadDiagnostic();
        RunButtonDiagnostic();
        RunSliderDiagnostic();

        //manta->Initialize();
    }
}

void MantaThread::RunPadDiagnostic()
{
    uint8_t effs[6];
    for(int i = 0; i < 6; ++i)
    {
       effs[i] = 0xff;
    }

    for(int i = 0; i < 6; ++i)
    {
       manta->SetPadLEDRow(Manta::Amber, i, 0xFF);
       waitForTransmitComplete(*manta);
       usleep(200000);
       manta->SetPadLEDRow(Manta::Red, i, 0xFF);
       waitForTransmitComplete(*manta);
       usleep(200000);
       manta->SetPadLEDRow(Manta::Amber, i, 0xFF);
       waitForTransmitComplete(*manta);
       usleep(200000);
       manta->SetPadLEDRow(Manta::Off, i, 0xFF);
       waitForTransmitComplete(*manta);
    }
    for(int i = 0; i < 8; ++i)
    {
       manta->SetPadLEDColumn(Manta::Amber, i, 0xFF);
       waitForTransmitComplete(*manta);
       usleep(200000);
       manta->SetPadLEDColumn(Manta::Red, i, 0xFF);
       waitForTransmitComplete(*manta);
       usleep(200000);
       manta->SetPadLEDColumn(Manta::Amber, i, 0xFF);
       waitForTransmitComplete(*manta);
       usleep(200000);
       manta->SetPadLEDColumn(Manta::Off, i, 0xFF);
       waitForTransmitComplete(*manta);
    }
    manta->SetPadLED(Manta::Red, 0);
    waitForTransmitComplete(*manta);
    usleep(100000);
    for(int i = 1; i < 48; ++i)
    {
       manta->SetPadLED(Manta::Red, i);
       manta->SetPadLED(Manta::Amber, i - 1);
       waitForTransmitComplete(*manta);
       usleep(100000);
    }

    // Clear all LEDs
    manta->SetPadLEDFrame(Manta::Off, effs);
    waitForTransmitComplete(*manta);
    usleep(250000);

    // Test all Amber LEDs
    manta->SetPadLEDFrame(Manta::Amber, effs);
    waitForTransmitComplete(*manta);
    usleep(250000);

    // Red
    manta->SetPadLEDFrame(Manta::Red, effs);
    waitForTransmitComplete(*manta);
    usleep(250000);

    // Clear
    manta->SetPadLEDFrame(Manta::Off, effs);
    waitForTransmitComplete(*manta);
    usleep(250000);
}

void MantaThread::RunButtonDiagnostic()
{
    for(int i = 0; i < 4; ++i)
    {
       manta->SetButtonLED(Manta::Amber, i);
       waitForTransmitComplete(*manta);
       usleep(250000);
       manta->SetButtonLED(Manta::Red, i);
       waitForTransmitComplete(*manta);
       usleep(250000);
       manta->SetButtonLED(Manta::Amber, i);
       waitForTransmitComplete(*manta);
       usleep(250000);
       manta->SetButtonLED(Manta::Off, i);
       waitForTransmitComplete(*manta);
    }
}

void MantaThread::RunSliderDiagnostic()
{
    for(int i = 0x80, j = 0x01; i; i >>= 1, j <<= 1)
    {
       manta->SetSliderLED(Manta::Off, 0, 0xFF);
       manta->SetSliderLED(Manta::Off, 1, 0xFF);
       manta->SetSliderLED(Manta::Amber, 0, i);
       manta->SetSliderLED(Manta::Amber, 1, j);
       waitForTransmitComplete(*manta);
       usleep(100000);
    }
    manta->SetSliderLED(Manta::Off, 0, 0xFF);
    manta->SetSliderLED(Manta::Off, 1, 0xFF);
    waitForTransmitComplete(*manta);
}

void MantaThread::run()
{
    forever
    {
        if (manta && m_options && CheckMantaConnected())
        {
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
