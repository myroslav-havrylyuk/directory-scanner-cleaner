#include "configfilehandler.h"

ConfigFileHandler::ConfigFileHandler(const QString &organization,
                                     const QString &application,
                                     QSettings::Scope scope,
                                     QSettings::Format format)
    : m_Organization(organization), m_Application(application), m_Scope(scope),
      m_Format(format) {
    readSettings();
}

QString ConfigFileHandler::getDeletionFilePath() const {
    return m_DeletionFilePath;
}

void ConfigFileHandler::setDeletionFilePath(QString newFilePath) {
    m_DeletionFilePath = newFilePath;
}

uint ConfigFileHandler::getRecursionDepth() const { return m_RecursionDepth; }

void ConfigFileHandler::setRecursionDepth(uint newDepth) {
    m_RecursionDepth = newDepth;
}

void ConfigFileHandler::readSettings() {
    QSettings settings(m_Format, m_Scope, m_Organization, m_Application);
    m_DeletionFilePath = settings.value("deletionHistoryFilePath").toString();
    m_RecursionDepth = settings.value("recursionDepth").toUInt();
}

void ConfigFileHandler::writeSettings() {
    QSettings settings(m_Format, m_Scope, m_Organization, m_Application);
    qDebug() << settings.fileName();
    settings.setValue("deletionHistoryFilePath", m_DeletionFilePath);
    settings.setValue("recursionDepth", m_RecursionDepth);
}
