#include "filemanagercontroller.h"

#include <QRegularExpression>
#include <QQmlEngine>
#include <QDir>

FileManagerController::FileManagerController(FileManagerModel &fileManagerModel)
    : m_FileManagerModel(fileManagerModel)
{
    QString initialRootPath = fileManagerModel.getRootPath();
    if(!initialRootPath.isEmpty()){
        setActivePath(fileManagerModel.getRootPath());
    }
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
    validActivePath.replace(QRegularExpression("\\\\"), "/");
    qDebug() << "Edited active path ahs been set: " << validActivePath;

    QDir activePath(validActivePath);
    if (!validActivePath.isEmpty() && activePath.exists())
    {
        m_ActivePath = validActivePath;
        m_FileManagerModel.setRootPath(validActivePath);
        emit activePathChanged();
    } else {
        //show warning message
        QQmlEngine engine;
        const QUrl url(u"qrc:/demo_app/warningmessage.qml"_qs);
        QQmlComponent component(&engine, url);
        QObject *object = component.create();
        emit activePathInvalid();
    }
}

QString FileManagerController::ActivePath() const
{
    QString activePath = m_ActivePath;
    activePath.replace(QRegularExpression("/"), "\\");
    return activePath;
}
