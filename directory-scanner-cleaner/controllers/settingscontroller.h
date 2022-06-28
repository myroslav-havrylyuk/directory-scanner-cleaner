#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QQuickItem>

class ConfigFileHandler;

class SettingsController : public QQuickItem
    {
        Q_OBJECT

        Q_PROPERTY(QString historyPath READ HistoryPathToView WRITE setHistoryPath NOTIFY historyPathChanged)
    public:
        SettingsController(ConfigFileHandler &configFileHandler);

        void setHistoryPath(const QString &newActivePath);
        QString HistoryPathToView() const;
        QString getHistoryPath();

    signals:
        void historyPathInvalid();
        void historyPathChanged();

public slots:
        void saveSettings();

    private:
        QString m_HistoryPath;
        ConfigFileHandler &m_ConfigFileModel;
};

#endif // SETTINGSCONTROLLER_H
