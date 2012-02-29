#ifndef _QMANTALOGGING_H
#define _QMANTALOGGING_H

#include <QString>

class MantaLogging
{
public:
    static void LogMessage(QString message, QString filename="manta.log");
};

#endif // _QMANTASETTINGS_H

