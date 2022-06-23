#include "settingscontroller.h"
#include "tools/configfilehandler.h"

#include <QQmlApplicationEngine>
#include <QRegularExpression>
#include <QQmlEngine>
#include <QDir>
#include <QQuickView>

extern QQmlApplicationEngine *gEngine;

SettingsController::SettingsController(ConfigFileHandler &configFileHandler)
    : m_ConfigFileModel(configFileHandler)
{
    m_HistoryPath = m_ConfigFileModel.getDeletionFilePath();
    m_warningMessage  = nullptr;
}

void SettingsController::setHistoryPath(const QString &newActivePath)
{
    qDebug() << "New active path has been set: " << newActivePath;
    QString validActivePath = newActivePath;
    validActivePath.remove(QRegularExpression("file:///"));
    validActivePath.replace(QRegularExpression("\\\\"), "/");
    qDebug() << "Edited active path has been set: " << validActivePath;

    QDir activePath(validActivePath);
    if (activePath.exists())
    {
        m_HistoryPath = validActivePath;
        emit historyPathChanged();
    } else {
        //show warning message
        const QUrl url(u"qrc:/directory-scanner-cleaner/views/warningmessage.qml"_qs);
        QQmlComponent component(gEngine, url);
        if(m_warningMessage != nullptr){
            delete m_warningMessage;
        }
        m_warningMessage = component.create();
        emit historyPathInvalid();
    }
}

QString SettingsController::HistoryPath() const
{
    QString activePath = m_HistoryPath;
    activePath.replace(QRegularExpression("/"), "\\");
    return activePath;
}

void SettingsController::setState(bool newState)
{
    if(newState)
    {
        m_HistoryPath = m_ConfigFileModel.getDeletionFilePath();
        emit historyPathChanged();
    }
    else
    {
        m_ConfigFileModel.setDeletionFilePath(m_HistoryPath);
        m_ConfigFileModel.writeSettings();
    }
}
