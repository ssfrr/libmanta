#ifndef MANTASETTINGSPARSER_H_
#define MANTASETTINGSPARSER_H_

#define _DEFINE_DEPRECATED_HASH_CLASSES 0

#include <string>
#include <map>

using namespace std;

const string trim(const string& pString, const string& pWhitespace = " \t");

class MantaMidiSettings;

class MantaSettingsParser
{
public:
    MantaSettingsParser(MantaMidiSettings *pSettings);
    bool ReadCollFile(const char *fileName);
    bool UpdateSettings();
    void PrintSettings();

private:
    bool IsValidKey(std::string key);
    void AssignKeyToValue(std::string key, std::string value);
    bool UpdateSetting(const string& key, const string& val);

    MantaMidiSettings *m_pSettings;
    map<string, string> m_configDefaults;
};

#endif
