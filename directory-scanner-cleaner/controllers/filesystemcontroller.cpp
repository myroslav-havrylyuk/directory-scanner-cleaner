#include "filesystemcontroller.h"
#include "models/filesystemmodel.h"

#include <QQmlApplicationEngine>
#include <QRegularExpression>
#include <QQmlEngine>
#include <QDir>
#include <QQuickView>

extern QQmlApplicationEngine *gEngine;

FileSystemController::FileSystemController(FileSystemModel &fileSystemModel)
    : m_FileSystemModel(fileSystemModel)
{
    m_warningMessage  = nullptr;
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
    validActivePath = QDir::cleanPath(validActivePath);
    qDebug() << "Edited active path has been set: " << validActivePath;

    QDir activePath(validActivePath);
    if (activePath.exists())
    {
        m_ActivePath = validActivePath;
        m_FileSystemModel.setRootPath(m_ActivePath);
        emit activePathChanged();
    } else {
        //show warning message
        const QUrl url(u"qrc:/directory-scanner-cleaner/views/warningmessage.qml"_qs);
        QQmlComponent component(gEngine, url);
        if(m_warningMessage != nullptr){
            delete m_warningMessage;
        }
        m_warningMessage = component.create();
        emit activePathInvalid();
    }
}

QString FileSystemController::ActivePath() const
{
    return QDir::toNativeSeparators(m_ActivePath);
}
