#include "filesystemcontroller.h"
#include "models/filesystemmodel.h"

#include <QRegularExpression>
#include <QQmlEngine>
#include <QDir>
#include <QQuickView>

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
        //show warning message
        QQmlEngine engine;
        const QUrl url(u"qrc:/directory-scanner-cleaner/warningmessage.qml"_qs);
        QQmlComponent component(&engine, url);
        QObject *object = component.create();
        /*QQuickView view;
        view.setSource(QUrl::fromLocalFile(u"qrc:/directory-scanner-cleaner/warningmessage.qml"_qs));
        view.show();*/
        emit activePathInvalid();
    }
}

QString FileSystemController::ActivePath() const
{
    QString activePath = m_ActivePath;
    activePath.replace(QRegularExpression("/"), "\\");
    return activePath;
}
