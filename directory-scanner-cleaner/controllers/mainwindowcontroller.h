#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include "settingscontroller.h"
#include "tools/configfilehandler.h"

#include <QObject>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>

extern QQmlApplicationEngine *gEngine;

class MainWindowController : public QObject
{
    Q_OBJECT

private:
    QObject *m_SettingsWindow = nullptr;
    QQmlComponent *m_Component = nullptr;
    QQmlContext *m_SettingsQmlContext = nullptr;
    SettingsController *m_SettingsController = nullptr;
    ConfigFileHandler *m_Handler = nullptr;
public:
    explicit MainWindowController(QObject *parent = nullptr);

signals:

public slots:
    void openSettingsWindow();
};

#endif // MAINWINDOWCONTROLLER_H
