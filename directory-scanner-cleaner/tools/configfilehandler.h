#ifndef CONFIGFILEHANDLER_H
#define CONFIGFILEHANDLER_H

#include <QString>
#include <QSettings>
#include <QCoreApplication>

class ConfigFileHandler
{
public:
    ConfigFileHandler(const QString &organization = "Some Organization",
                      const QString &application = "Directory Scanner & Cleaner",
                      QSettings::Scope scope = QSettings::SystemScope,
                      QSettings::Format format = QSettings::IniFormat);

    QString getDeletionFilePath() const;
    void setDeletionFilePath(QString filePath);
    uint getRecursionDepth() const;
    void setRecursionDepth(uint filePath);
    void readSettings();
    void writeSettings();

private:
QString m_Organization;
QString m_Application;
QSettings::Scope m_Scope;
QSettings::Format m_Format;

QString m_DeletionFilePath;
uint m_RecursionDepth;
};

#endif // CONFIGFILEHANDLER_H
