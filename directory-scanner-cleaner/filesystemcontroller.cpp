#include "filesystemcontroller.h"

#include <QRegularExpression>
#include <QQmlEngine>
#include <QDir>

FileSystemController::FileSystemController()
{
}

void FileSystemController::setActivePath(const QString &newActivePath)
{
    qDebug() << "New active path has been set: " << newActivePath;
    QString validActivePath = newActivePath;
    validActivePath.remove(QRegularExpression("file:///"));
    validActivePath.replace(QRegularExpression("\\\\"), "/");
    qDebug() << "Edited active path has been set: " << validActivePath;

    QDir activePath(validActivePath);
    if (activePath.exists())
    {
        m_ActivePath = validActivePath;
        emit activePathChanged();
    } else {
        emit activePathInvalid();
    }
}

QString FileSystemController::ActivePath() const
{
    QString activePath = m_ActivePath;
    activePath.replace(QRegularExpression("/"), "\\");
    return activePath;
}
