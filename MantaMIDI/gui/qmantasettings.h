#ifndef _QMANTASETTINGS_H
#define _QMANTASETTINGS_H

#include "../settings/MantaMidiSettings.h"
#include <QString>

class QMantaMidiSettings : public MantaMidiSettings
{
public:
    QMantaMidiSettings();
    QMantaMidiSettings(int argc, char **argv);

    bool Save(QString filename);
    bool Load(QString filename);
};

#endif // _QMANTASETTINGS_H
