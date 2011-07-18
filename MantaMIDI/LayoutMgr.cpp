#include "LayoutMgr.h"

#include <QFile>
#include <QTextStream>

LayoutMgr::LayoutMgr()
{
}

LayoutMgr::~LayoutMgr()
{

}

bool LayoutMgr::ReadLayoutFile(QString &filePath/*, int **arr, int arrLen*/)
{
    QFile inFile(filePath);
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    int line = 0;
    QTextStream in(&inFile);
    while (!in.atEnd()/* && line < arrLen*/)
    {
        QString line = in.readLine();

        //arr[line] = line.toInt();
    }

    return line > 0;
}

bool LayoutMgr::WriteLayoutFile(QString &filePath/*, int arr[], int arrLen*/)
{
    return false;
}

