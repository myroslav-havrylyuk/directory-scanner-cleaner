#ifndef DELETIONHISTORYHANDLER_H
#define DELETIONHISTORYHANDLER_H

#include <QString>

class DeletionHistoryHandler
{
public:
    DeletionHistoryHandler();

    void appendFile();
private:
    QString m_FileName;
};

#endif // DELETIONHISTORYHANDLER_H
