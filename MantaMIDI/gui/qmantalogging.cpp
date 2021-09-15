#include "qmantalogging.h"
#include <QFile>
#include <QDataStream>

void MantaLogging::LogMessage(QString message, QString filename)
{
    QFile file(filename);
    file.open(QIODevice::Append);
    QDataStream out(&file);   // we will serialize the data into the file

    out << message << "\n";

    file.close();
}
