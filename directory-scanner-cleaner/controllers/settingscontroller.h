#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QQuickItem>

class ConfigFileHandler;

class SettingsController : public QQuickItem
    {
        Q_OBJECT

        Q_PROPERTY(QString historyPath READ HistoryPath WRITE setHistoryPath NOTIFY historyPathChanged)
        Q_PROPERTY(bool state WRITE setState)
    public:
        SettingsController(ConfigFileHandler &configFileHandler);

        void setHistoryPath(const QString &newActivePath);
        void setState(bool newState);
        QString HistoryPath() const;
        bool State() const;

    signals:
        void historyPathInvalid();
        void historyPathChanged();

    private:
        QString m_HistoryPath;
        ConfigFileHandler &m_ConfigFileModel;
        QObject *m_warningMessage;
};

#endif // SETTINGSCONTROLLER_H
