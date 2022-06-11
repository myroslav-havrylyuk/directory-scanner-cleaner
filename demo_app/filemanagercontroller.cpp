#include "filemanagercontroller.h"

#include <QRegularExpression>
#include <QQmlEngine>
#include <QDir>

FileManagerController::FileManagerController(FileManagerModel &fileManagerModel)
    : m_FileManagerModel(fileManagerModel)
{

}

void FileManagerController::registerMe()
{
    qmlRegisterType<FileManagerController>("FileManager", 1, 0, "FileManagerController");
}

void FileManagerController::setActivePath(const QString &newActivePath)
{
    qDebug() << "New active path ahs been set: " << newActivePath;
    QString validActivePath = newActivePath;
    validActivePath.remove(QRegularExpression("file:///"));
    qDebug() << "Edited active path ahs been set: " << validActivePath;

    QDir activePath(validActivePath);
    if (activePath.exists())
    {
        m_ActivePath = validActivePath;
        m_FileManagerModel.setRootPath(validActivePath);
        emit ActivePathValueChanged();
    } else {
        emit activePathInvalid();
    }
}

const QString &FileManagerController::ActivePath() const
{
    return m_ActivePath;
}
