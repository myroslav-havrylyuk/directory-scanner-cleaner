#ifndef FILEMANAGERCONTROLLER_H
#define FILEMANAGERCONTROLLER_H

#include <QObject>
#include <QQuickItem>
#include <QString>

#include "filemanagermodel.h"

class FileManagerController : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QString activePath READ ActivePath WRITE setActivePath NOTIFY ActivePathValueChanged)
public:
    FileManagerController();
    static void registerMe();

    void setActivePath(const QString &newActivePath);
    const QString &ActivePath() const;

signals:
    void activePathInvalid();
    void ActivePathValueChanged();

private:
    QString m_ActivePath;
    FileManagerModel m_FileManagerModel;
};

#endif // FILEMANAGERCONTROLLER_H
