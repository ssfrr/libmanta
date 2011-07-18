#include "qmantasettings.h"
#include <QFile>
#include <QDataStream>

QMantaMidiSettings::QMantaMidiSettings() :
    MantaMidiSettings()
{
}

QMantaMidiSettings::QMantaMidiSettings(int argc, char **argv) :
    MantaMidiSettings(argc, argv)
{

}

#define MAGIC_NUM           0xAF001242
#define MANTAMIDI_VERSION   1
bool QMantaMidiSettings::Save(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);   // we will serialize the data into the file

    // Write header with magic number and version
    out << (quint32)MAGIC_NUM;
    out << (quint32)MANTAMIDI_VERSION;

    out.setVersion(QDataStream::Qt_4_7);

    // Serialize Master settings
    out << m_bDebugMode;
    out << m_bUseVelocity;

    // Serialize Pad Settings
    out << (quint8)m_padLayout;
    out << (quint8)m_padMode;

    for (unsigned char i = 0; i < numPads; ++i)
    {
        out << (quint8)m_padEventChannel[i];
        out << (quint8)m_basePadMidi[i];
        out << (quint8)m_padLEDChannel[i];
        out << (quint8)m_AmberLEDMidi[i];
        out << (quint8)m_RedLEDMidi[i];
        out << (quint8)m_inactivePadColor[i];
        out << (quint8)m_onPadColor[i];
        out << (quint8)m_offPadColor[i];
    }

    // serialize slider settings
    for (unsigned char i = 0; i < numSliders; ++i)
    {
        out << (quint8)m_slider_EventChannel[i];
        out << (quint8)m_slider_MidiNote[i];
        out << (quint8)m_slider_Mode[i];
    }

    // Serialize Button settings
    for (unsigned char i = 0; i < numButtons; ++i)
    {
        out << (quint8)m_buttonMode[i];
        out << (quint8)m_buttonEventChannel[i];
        out << (quint8)m_buttonMidi[i];
        out << (quint8)m_inactiveButtonColor[i];
        out << (quint8)m_onButtonColor[i];
        out << (quint8)m_offButtonColor[i];
    }

    file.close();
    return true;
}

bool QMantaMidiSettings::Load(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);

    quint8 readByte = 0;

    quint32 magic;
    in >> magic;
    if (magic != MAGIC_NUM)
        return false;

    quint32 version;
    in >> version;
    if (version != MANTAMIDI_VERSION)
        return false;

    // Read Master settings
    in >> m_bDebugMode;
    in >> m_bUseVelocity;

    // Read Pad Settings
    in >> readByte; m_padLayout = (PadLayout)readByte;
    in >> readByte; m_padMode = (PadValMode)readByte;

    for (unsigned char i = 0; i < numPads; ++i)
    {
        in >> readByte; m_padEventChannel[i] = readByte;
        in >> readByte; m_basePadMidi[i] = readByte;
        in >> readByte; m_padLEDChannel[i] = readByte;
        in >> readByte; m_AmberLEDMidi[i] = readByte;
        in >> readByte; m_RedLEDMidi[i] = readByte;
        in >> readByte; m_inactivePadColor[i] = (Manta::LEDState)readByte;
        in >> readByte; m_onPadColor[i] = (Manta::LEDState)readByte;
        in >> readByte; m_offPadColor[i] = (Manta::LEDState)readByte;
    }

    // serialize slider settings
    for (unsigned char i = 0; i < numSliders; ++i)
    {
        in >> readByte; m_slider_EventChannel[i] = readByte;
        in >> readByte; m_slider_MidiNote[i] = readByte;
        in >> readByte; m_slider_Mode[i] = (SliderMode)readByte;
    }

    // Serialize Button settings
    for (unsigned char i = 0; i < numButtons; ++i)
    {
        in >> readByte; m_buttonMode[i] = (ButtonMode)readByte;
        in >> readByte; m_buttonEventChannel[i] = readByte;
        in >> readByte; m_buttonMidi[i] = readByte;
        in >> readByte; m_inactiveButtonColor[i] = (Manta::LEDState)readByte;
        in >> readByte; m_onButtonColor[i] = (Manta::LEDState)readByte;
        in >> readByte; m_offButtonColor[i] = (Manta::LEDState)readByte;
    }

    file.close();
    return false;
}
