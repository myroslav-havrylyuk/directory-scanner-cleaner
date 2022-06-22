#ifndef FILESYSTEMCONTROLLER_H
#define FILESYSTEMCONTROLLER_H

#include <QObject>
#include <QQuickItem>
#include <QString>

class FileSystemModel;

class FileSystemController : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QString activePath READ ActivePath WRITE setActivePath NOTIFY activePathChanged)
public:
    FileSystemController(FileSystemModel &fileSystemModel);

    QString ActivePath() const;
    void setActivePath(const QString &newActivePath);

signals:
    void activePathInvalid();
    void activePathChanged();

private:
    QString m_ActivePath;
    FileSystemModel &m_FileSystemModel;
};

#endif // FILESYSTEMCONTROLLER_H
