#include "filesystemcontroller.h"
#include "models/filesystemmodel.h"

#include <QQmlApplicationEngine>
#include <QRegularExpression>
#include <QQmlEngine>
#include <QDir>
#include <QQuickView>

FileSystemController::FileSystemController(FileSystemModel &fileSystemModel)
    : m_FileSystemModel(fileSystemModel)
{
    QString initialRootPath = fileSystemModel.getRootPath();
    if(!initialRootPath.isEmpty()){
        setActivePath(initialRootPath);
    }
    connect(this, &FileSystemController::cancelSetupModel, &m_FileSystemModel, &FileSystemModel::cancelSetupModelHandler);
    connect(&m_FileSystemModel, &FileSystemModel::setupModelCanceled, this, &FileSystemController::setupModelCanceledHandler);
}

void FileSystemController::setActivePath(const QString &newActivePath)
{
    qDebug() << "setActivePath function has been called";
    QString validActivePath = newActivePath;
    validActivePath.remove(QRegularExpression("file:///"));
    validActivePath = QDir::cleanPath(validActivePath);
    if(m_ActivePath != validActivePath){
        qDebug() << "New active path has been set: " << newActivePath;
        qDebug() << "Edited active path has been set: " << validActivePath;

        QDir activePath(validActivePath);
        if (activePath.exists())
        {
            m_ActivePath = validActivePath;
            m_FileSystemModel.setupModel(m_ActivePath);
            emit activePathChanged();
        } else {
            qDebug() << "invalid path has been entered";
            emit activePathInvalid();
        }
    }
}

void FileSystemController::cancelSetupModelHandler()
{
    emit cancelSetupModel();
    qDebug() << "cancel setup model handler in FileSystemController";
}

void FileSystemController::setupModelCanceledHandler()
{
    emit setupModelCanceled();
    qDebug() << "setup model canceled handler in FileSystemController";
}

QString FileSystemController::ActivePath() const
{
    return QDir::toNativeSeparators(m_ActivePath);
}
