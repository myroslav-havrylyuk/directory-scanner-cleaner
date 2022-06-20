#include "configfilehandler.h"

ConfigFileHandler::ConfigFileHandler(const QString &organization,
                                     const QString &application,
                                     QSettings::Scope scope,
                                     QSettings::Format format):
    m_Organization(organization),
    m_Application(application),
    m_Scope(scope),
    m_Format(format)
{
    readSettings();
}

QString ConfigFileHandler::getDeletionFilePath() const
{
    return m_DeletionFilePath;
}
void ConfigFileHandler::setDeletionFilePath(QString newFilePath)
{
    m_DeletionFilePath = newFilePath;
}
void ConfigFileHandler::readSettings()
{
    QSettings settings(m_Format, m_Scope, m_Organization, m_Application);
    m_DeletionFilePath = settings.value("deletionHistoryFilePath").toString();
}
void ConfigFileHandler::writeSettings()
{
    QSettings settings(m_Format, m_Scope, m_Organization, m_Application);
    settings.setValue("deletionHistoryFilePath", m_DeletionFilePath);
}
