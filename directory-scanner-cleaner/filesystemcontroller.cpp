#include "filesystemcontroller.h"
#include "filesystemmodel.h"

#include <QRegularExpression>
#include <QQmlEngine>
#include <QDir>

FileSystemController::FileSystemController(FileSystemModel &fileSystemModel)
    : m_FileSystemModel(fileSystemModel)
{
    QString initialRootPath = fileSystemModel.getRootPath();
    if(!initialRootPath.isEmpty()){
        setActivePath(fileSystemModel.getRootPath());
    }
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
        m_FileSystemModel.setRootPath(m_ActivePath);
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
