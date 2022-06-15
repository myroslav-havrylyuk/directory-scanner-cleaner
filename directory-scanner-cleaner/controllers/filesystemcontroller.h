#ifndef FILEMANAGERCONTROLLER_H
#define FILEMANAGERCONTROLLER_H

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

    void setActivePath(const QString &newActivePath);
    QString ActivePath() const;

signals:
    void activePathInvalid();
    void activePathChanged();

private:
    QString m_ActivePath;
    FileSystemModel &m_FileSystemModel;
};

#endif // FILEMANAGERCONTROLLER_H
