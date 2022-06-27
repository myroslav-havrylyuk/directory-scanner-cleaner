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
    validActivePath = QDir::cleanPath(validActivePath);
    qDebug() << "Edited active path has been set: " << validActivePath;

    QDir activePath(validActivePath);
    if (activePath.exists())
    {
        m_HistoryPath = validActivePath;
        emit historyPathChanged();
    } else {
        //show warning message
        qDebug() << "invalid path has been entered";
        emit historyPathInvalid();
    }
}

QString SettingsController::HistoryPath() const
{
    return QDir::toNativeSeparators(m_HistoryPath);;
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
