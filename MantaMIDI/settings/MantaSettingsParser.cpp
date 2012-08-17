#include "MantaSettingsParser.h"
#include "MantaMidiSettings.h"
#include <iostream>
#include <fstream>

#define DEFINE_CONFIG_DEFAULT(key, value)          m_configDefaults[key] = value

// stackoverflow
const string trim(const string& pString, const string& pWhitespace)
{
    const size_t beginStr = pString.find_first_not_of(pWhitespace);
    if (beginStr == string::npos)
    {
        // no content
        return "";
    }

    const size_t endStr = pString.find_last_not_of(pWhitespace);
    const size_t range = endStr - beginStr + 1;

    return pString.substr(beginStr, range);
}

MantaSettingsParser::MantaSettingsParser(MantaMidiSettings *pSettings)
{
    m_pSettings = pSettings;

    // Master Settings
    DEFINE_CONFIG_DEFAULT("Velocity", "1");

    // Pad Settings
    DEFINE_CONFIG_DEFAULT("Pad_Mode", "0");
    //DEFINE_CONFIG_DEFAULT("Pad_Layout", "3"); // Duet
    DEFINE_CONFIG_DEFAULT("Pad_MonoCC", "11");
    DEFINE_CONFIG_DEFAULT("Pad_InactiveColor", "0");
    DEFINE_CONFIG_DEFAULT("Pad_OnColor", "1");
    DEFINE_CONFIG_DEFAULT("Pad_OffColor", "0");

    // Master Pad Settings

    char extra[16];
    char strResult[64];
    // Individual Pad Settings
    for(int i=1; i <= 48; ++i)
    {
        sprintf(strResult, "Pad_%d_Channel", i);
        DEFINE_CONFIG_DEFAULT(strResult, "1");

        sprintf(strResult, "Pad_%d_MIDINote", i);
        sprintf(extra, "%d", i + 32);
        DEFINE_CONFIG_DEFAULT(strResult, extra);
    }

    /* Pad Receive */
    /*unsigned char m_padLEDChannel[numPads];
    unsigned char m_AmberLEDMidi[numPads];
    unsigned char m_RedLEDMidi[numPads];*/

    // Button Settings
    for (int i = 1; i <= 4; ++i)
    {
        sprintf(strResult, "Button_%d_MIDI", i);
        sprintf(extra, "%d", 102 + i);
        DEFINE_CONFIG_DEFAULT(strResult, extra);

        sprintf(strResult, "Button_%d_Channel", i);
        sprintf(extra, "%d", 1);
        DEFINE_CONFIG_DEFAULT(strResult, extra);

        sprintf(strResult, "Button_%d_Mode", i);
        sprintf(extra, "%d", 1);
        DEFINE_CONFIG_DEFAULT(strResult, extra);

        sprintf(strResult, "Button_%d_InactiveColor", i);
        sprintf(extra, "%d", 0);
        DEFINE_CONFIG_DEFAULT(strResult, extra);

        sprintf(strResult, "Button_%d_OnColor", i);
        sprintf(extra, "%d", 1);
        DEFINE_CONFIG_DEFAULT(strResult, extra);

        sprintf(strResult, "Button_%d_OffColor", i);
        sprintf(extra, "%d", 0);
        DEFINE_CONFIG_DEFAULT(strResult, extra);
    }

    // Slider Settings
    /* Sliders */
    DEFINE_CONFIG_DEFAULT("Slider_0_Channel", "11");
    DEFINE_CONFIG_DEFAULT("Slider_1_Channel", "0");
    DEFINE_CONFIG_DEFAULT("Slider_0_MIDI", "1");
    DEFINE_CONFIG_DEFAULT("Slider_1_MIDI", "2");
    DEFINE_CONFIG_DEFAULT("Slider_0_Mode", "0");
    DEFINE_CONFIG_DEFAULT("Slider_1_Mode", "0");
}

bool MantaSettingsParser::ReadCollFile(const char *fileName)
{
    cout << "Reading " << fileName << " as settings file" << endl;
    string line;
    ifstream fin;
    fin.open(fileName);

    if (fin.is_open())
    {
        int len = 0;
        int commaPos = 0;
        int colonPos = 0;

        while( fin.good() )
        {
            getline(fin, line);
            len = line.length();
            commaPos = line.find(',');
            colonPos = line.find(';');

            string key = trim(line.substr(0, commaPos));
            string value = trim(line.substr(commaPos + 1, colonPos - commaPos - 1));

            if (IsValidKey(key))
                AssignKeyToValue(key, value);
            else
                cout << "PARSE ERROR: Invalid key \"" << key << "\"" << endl;
        }
    }
    else
        cout << "No file of this name" << endl;

    fin.close();

    return false;
}

bool MantaSettingsParser::IsValidKey(string key)
{   
    if (m_configDefaults.count(key) > 0)
        return true;
    else
        return false;
}

void MantaSettingsParser::AssignKeyToValue(string key, string value)
{
    m_configDefaults[key] = value;
}

bool MantaSettingsParser::UpdateSettings()
{
    bool bRet = true;

    for(map<string, string>::iterator itr = m_configDefaults.begin(); itr != m_configDefaults.end(); ++itr)
    {
        string key = (*itr).first;
        string val = (*itr).second;

        bRet &= UpdateSetting(key, val);
    }

    return bRet;
}

bool MantaSettingsParser::UpdateSetting(const string& key, const string& val)
{
    bool bRet = false;
    bool debug = m_pSettings->GetDebugMode();

    int iVal = atoi(val.c_str());
    if (IsValidKey(key))
    {
        const size_t _first = key.find_first_of("_");
        const size_t _last = key.find_last_of("_");
        string type = key.substr(0, _first);
        string function = key.substr(_last + 1);
        unsigned long index = string::npos;
        if (_first != _last)
            index = atoi(key.substr(_first + 1, (_last - _first - 1)).c_str());

        if (type == "Pad")
        {
            if (function == "Mode")
                m_pSettings->SetPad_Mode((PadValMode)iVal);
            else if (function == "Layout")
                m_pSettings->SetPad_Layout((PadLayout)iVal);
            else if (function == "MonoCC")
                m_pSettings->SetPad_MonoCCNumber(iVal);
            else if (function == "InactiveColor")
                m_pSettings->SetAllPadInactiveColor((Manta::LEDState)iVal);
            else if (function == "OnColor")
                m_pSettings->SetAllPadOnColor((Manta::LEDState)iVal);
            else if (function == "OffColor")
                m_pSettings->SetAllPadOffColor((Manta::LEDState)iVal);
            else if (function == "Channel")
                m_pSettings->SetPadLED_MidiChannel(index - 1, iVal);
            else if (function == "MIDINote")
                m_pSettings->SetPad_MIDINote(index - 1, iVal);
            else
            {
                if (debug) cout << "Invalid Pad Setting!!!" << endl;
            }
        }
        else if (type == "Button")
        {
            if (function == "MIDI")
            {
                if (index == 1 || index == string::npos)
                    m_pSettings->SetButton_Midi(0, iVal);
                else if (index == 2 || index == string::npos)
                    m_pSettings->SetButton_Midi(1, iVal);
                else if (index == 3 || index == string::npos)
                    m_pSettings->SetButton_Midi(2, iVal);
                else if (index == 4 || index == string::npos)
                    m_pSettings->SetButton_Midi(3, iVal);
            }
            else if (function == "Channel")
            {
                if (index == 1 || index == string::npos)
                    m_pSettings->SetButton_Channel(0, iVal);
                else if (index == 2 || index == string::npos)
                    m_pSettings->SetButton_Channel(1, iVal);
                else if (index == 3 || index == string::npos)
                    m_pSettings->SetButton_Channel(2, iVal);
                else if (index == 4 || index == string::npos)
                    m_pSettings->SetButton_Channel(3, iVal);
            }
            else if (function == "Mode")
            {
                if (index == 1 || index == string::npos)
                    m_pSettings->SetButton_Mode(0, (ButtonMode)iVal);
                else if (index == 2 || index == string::npos)
                    m_pSettings->SetButton_Mode(1, (ButtonMode)iVal);
                else if (index == 3 || index == string::npos)
                    m_pSettings->SetButton_Mode(2, (ButtonMode)iVal);
                else if (index == 4 || index == string::npos)
                    m_pSettings->SetButton_Mode(3, (ButtonMode)iVal);
            }
            else if (function == "InactiveColor")
            {
                if (index == 1 || index == string::npos)
                    m_pSettings->SetButton_InactiveColor(0, (Manta::LEDState)iVal);
                else if (index == 2 || index == string::npos)
                    m_pSettings->SetButton_InactiveColor(1, (Manta::LEDState)iVal);
                else if (index == 3 || index == string::npos)
                    m_pSettings->SetButton_InactiveColor(2, (Manta::LEDState)iVal);
                else if (index == 4 || index == string::npos)
                    m_pSettings->SetButton_InactiveColor(3, (Manta::LEDState)iVal);
            }
            else if (function == "OnColor")
            {
                if (index == 1 || index == string::npos)
                    m_pSettings->SetButton_OnColor(0, (Manta::LEDState)iVal);
                else if (index == 2 || index == string::npos)
                    m_pSettings->SetButton_OnColor(1, (Manta::LEDState)iVal);
                else if (index == 3 || index == string::npos)
                    m_pSettings->SetButton_OnColor(2, (Manta::LEDState)iVal);
                else if (index == 4 || index == string::npos)
                    m_pSettings->SetButton_OnColor(3, (Manta::LEDState)iVal);
            }
            else if (function == "OffColor")
            {
                if (index == 1 || index == string::npos)
                    m_pSettings->SetButton_OffColor(0, (Manta::LEDState)iVal);
                else if (index == 2 || index == string::npos)
                    m_pSettings->SetButton_OffColor(1, (Manta::LEDState)iVal);
                else if (index == 3 || index == string::npos)
                    m_pSettings->SetButton_OffColor(2, (Manta::LEDState)iVal);
                else if (index == 4 || index == string::npos)
                    m_pSettings->SetButton_OffColor(3, (Manta::LEDState)iVal);
            }
        }
        else if (type == "Slider")
        {
            if (function == "Channel")
            {
                if (index == 0 || index == string::npos)
                    m_pSettings->SetSlider_Channel(0, iVal);
                else if (index == 1 || index == string::npos)
                    m_pSettings->SetSlider_Channel(1, iVal);
            }
            else if (function == "MIDI")
            {
                if (index == 0 || index == string::npos)
                    m_pSettings->SetSlider_Midi(0, iVal);
                else if (index == 1 || index == string::npos)
                    m_pSettings->SetSlider_Midi(1, iVal);
            }
            else if (function == "Mode")
            {
                if (index == 0 || index == string::npos)
                    m_pSettings->SetSlider_Mode(0, (SliderMode)iVal);
                else if (index == 1 || index == string::npos)
                    m_pSettings->SetSlider_Mode(1, (SliderMode)iVal);
            }
        }
        else
        {
            if (key == "Velocity")
                m_pSettings->SetUseVelocity(iVal);
        }
    }

    return bRet;
}

void MantaSettingsParser::PrintSettings()
{
    map<string, string>::iterator itr;

    for(itr = m_configDefaults.begin(); itr != m_configDefaults.end(); ++itr)
        cout << (*itr).first << ", " << (*itr).second << ";" << endl;
}