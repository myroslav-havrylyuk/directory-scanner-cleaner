#include "filemanagercontroller.h"

#include <QQmlEngine>
#include <QDir>

FileManagerController::FileManagerController()
{

}

void FileManagerController::registerMe()
{
    qmlRegisterType<FileManagerController>("FileManager", 1, 0, "FileManagerController");
}

void FileManagerController::setActivePath(const QString &newActivePath)
{
    QDir activePath(newActivePath);
    if (activePath.exists())
    {
        m_ActivePath = newActivePath;
        emit ActivePathValueChanged();
    } else {
        emit activePathInvalid();
    }
}

const QString &FileManagerController::ActivePath() const
{
    return m_ActivePath;
}
