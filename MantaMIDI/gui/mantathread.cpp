#include "mantathread.h"

MantaThread::MantaThread(QObject *parent) :
    QThread(parent), m_options(NULL), manta(NULL)
{
    m_runningMode = eMode_Disabled;

    connect(parent, SIGNAL(MantaDiagnosticsSignal()),
           this, SLOT(RunMantaDiagnostic()), Qt::QueuedConnection);
    connect(parent, SIGNAL(MantaDisconnectSignal()),
            this, SLOT(ForceMantaDisconnect()), Qt::QueuedConnection);
    connect(parent, SIGNAL(MantaCalibrateSignal()),
            this, SLOT(RunCalibrateMode()), Qt::QueuedConnection);
}

MantaThread::~MantaThread()
{
    /*if (manta)
    {
        if (this->isRunning())
            this->exit(0);

        if (manta->IsConnected())
        {
            //mutex.lock();
            //manta->Disconnect();
            //condition.wait(&mutex);
            //mutex.unlock();
        }

        delete manta;
    }*/
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

void MantaThread::ForceMantaDisconnect()
{
    /*if (manta->IsConnected())
    {
        Manta::HandleEvents();

        m_runningMode = eMode_Disabled;
        //manta->Disconnect();

        emit MantaConnectedMessage("Manta Force Disconnected");
    }*/
}

void MantaThread::RunMantaDiagnostic()
{
    m_runningMode = eMode_Diagnostic;
}

void MantaThread::CalibrateMode()
{
    bool bState = manta->GetCalibrationState();

    if (bState)
    {
        emit UpdateStatusMessage("Calibration disabled.");
        manta->SetCalibrateMode(false);
    }
    else
    {
        emit UpdateStatusMessage("Calibration Mode: Detecting max sensor values");
        manta->SetCalibrateMode(true);
    }
}

void MantaThread::RunDiagnostic()
{
    manta->SetLEDControl(Manta::PadAndButton, true);
    Manta::HandleEvents();
    manta->SetLEDControl(Manta::Slider, true);
    Manta::HandleEvents();

    RunPadDiagnostic();
    RunButtonDiagnostic();
    RunSliderDiagnostic();

    manta->Initialize();
    m_runningMode = eMode_Run;
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

void MantaThread::RunManta()
{
    if (manta && m_options && CheckMantaConnected())
    {
        try
        {
            manta->Initialize();
            forever
            {
                if (m_runningMode == eMode_Run)
                    manta->HandleEvents();
                else if (m_runningMode == eMode_Diagnostic)
                    RunDiagnostic();
                else if (m_runningMode == eMode_Disabled)
                    usleep(200);
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
}

void MantaThread::run()
{
    m_runningMode = eMode_Run;
    forever
    {
        RunManta();
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
       Manta::HandleEvents();
       usleep(200000);
       manta->SetPadLEDRow(Manta::Red, i, 0xFF);
       Manta::HandleEvents();
       usleep(200000);
       manta->SetPadLEDRow(Manta::Amber, i, 0xFF);
       Manta::HandleEvents();
       usleep(200000);
       manta->SetPadLEDRow(Manta::Off, i, 0xFF);
       Manta::HandleEvents();
    }
    for(int i = 0; i < 8; ++i)
    {
       manta->SetPadLEDColumn(Manta::Amber, i, 0xFF);
       Manta::HandleEvents();
       usleep(200000);
       manta->SetPadLEDColumn(Manta::Red, i, 0xFF);
       Manta::HandleEvents();
       usleep(200000);
       manta->SetPadLEDColumn(Manta::Amber, i, 0xFF);
       Manta::HandleEvents();
       usleep(200000);
       manta->SetPadLEDColumn(Manta::Off, i, 0xFF);
       Manta::HandleEvents();
    }
    manta->SetPadLED(Manta::Red, 0);
    Manta::HandleEvents();
    usleep(100000);
    for(int i = 1; i < 48; ++i)
    {
       manta->SetPadLED(Manta::Red, i);
       manta->SetPadLED(Manta::Amber, i - 1);
       Manta::HandleEvents();
       usleep(100000);
    }

    // Clear all LEDs
    manta->SetPadLEDFrame(Manta::Off, effs);
    Manta::HandleEvents();
    usleep(250000);

    // Test all Amber LEDs
    manta->SetPadLEDFrame(Manta::Amber, effs);
    Manta::HandleEvents();
    usleep(250000);

    // Red
    manta->SetPadLEDFrame(Manta::Red, effs);
    Manta::HandleEvents();
    usleep(250000);

    // Clear
    manta->SetPadLEDFrame(Manta::Off, effs);
    Manta::HandleEvents();
    usleep(250000);
}

void MantaThread::RunButtonDiagnostic()
{
    for(int i = 0; i < 4; ++i)
    {
       manta->SetButtonLED(Manta::Amber, i);
       Manta::HandleEvents();
       usleep(250000);
       manta->SetButtonLED(Manta::Red, i);
       Manta::HandleEvents();
       usleep(250000);
       manta->SetButtonLED(Manta::Amber, i);
       Manta::HandleEvents();
       usleep(250000);
       manta->SetButtonLED(Manta::Off, i);
       Manta::HandleEvents();
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
       Manta::HandleEvents();
       usleep(100000);
    }
    manta->SetSliderLED(Manta::Off, 0, 0xFF);
    manta->SetSliderLED(Manta::Off, 1, 0xFF);
    Manta::HandleEvents();
}
