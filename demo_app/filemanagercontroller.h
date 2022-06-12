#ifndef FILEMANAGERCONTROLLER_H
#define FILEMANAGERCONTROLLER_H

#include <QObject>
#include <QQuickItem>
#include <QString>

#include "filemanagermodel.h"

class FileManagerController : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QString activePath READ ActivePath WRITE setActivePath NOTIFY activePathChanged)
public:
    FileManagerController(FileManagerModel &fileManagerModel);
    static void registerMe();

    void setActivePath(const QString &newActivePath);
    QString ActivePath() const;

    FileManagerModel fileManagerModel();

signals:
    void activePathInvalid();
    void activePathChanged();

private:
    QString m_ActivePath;
    FileManagerModel &m_FileManagerModel;
};

#endif // FILEMANAGERCONTROLLER_H
