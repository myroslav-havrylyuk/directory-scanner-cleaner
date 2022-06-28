#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QQuickItem>

class ConfigFileHandler;

class SettingsController : public QQuickItem
    {
        Q_OBJECT

        Q_PROPERTY(QString historyPath READ HistoryPathToView WRITE setHistoryPath NOTIFY historyPathChanged)
        Q_PROPERTY(QString recursionDepth READ RecursionDepthToView WRITE setRecursionDepth NOTIFY recursionDepthChanged)
    public:
        SettingsController(ConfigFileHandler &configFileHandler);

        void setHistoryPath(const QString &newActivePath);
        void setRecursionDepth(QString &newDepth);
        QString HistoryPathToView() const;
        QString RecursionDepthToView() const;
        QString getHistoryPath();

    signals:
        void historyPathInvalid();
        void historyPathChanged();
        void recursionDepthChanged();

public slots:
        void saveSettings();

    private:
        QString m_HistoryPath;
        uint m_RecursionDepth;
        ConfigFileHandler &m_ConfigFileModel;
};

#endif // SETTINGSCONTROLLER_H
