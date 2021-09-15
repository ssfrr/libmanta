#ifndef _LAYOUTMGR_H
#define _LAYOUTMGR_H

#include <QString>

class LayoutMgr
{
public:
    LayoutMgr();
    ~LayoutMgr();

    bool ReadLayoutFile(QString &filePath/*, int **arr, int arrLen*/);
    bool WriteLayoutFile(QString &filePath/*, int arr[], int arrLen*/);

private:
    int m_layoutArr[48];
};

#endif
