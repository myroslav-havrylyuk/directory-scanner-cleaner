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

public slots:
    void cancelSetupModelHandler();
    void setupModelCanceledHandler();

signals:
    void activePathInvalid();
    void activePathChanged();
    void cancelSetupModel();
    void setupModelCanceled();

private:
    QString m_ActivePath;
    FileSystemModel &m_FileSystemModel;
};

#endif // FILESYSTEMCONTROLLER_H
