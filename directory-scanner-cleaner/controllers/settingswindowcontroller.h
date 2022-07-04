#ifndef SETTINGSWINDOWCONTROLLER_H
#define SETTINGSWINDOWCONTROLLER_H

#include "settingscontroller.h"
#include "tools/configfilehandler.h"

#include <QDebug>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>

extern SettingsController *gSettingsController;
extern QQmlApplicationEngine *gEngine;

class SettingsWindowController : public QObject {
    Q_OBJECT

private:
    QObject *m_SettingsWindow = nullptr;
    QQmlComponent *m_SettingsWindowComponent = nullptr;
    QQmlContext *m_SettingsQmlContext = nullptr;

public:
    explicit SettingsWindowController(QObject *parent = nullptr);
    ~SettingsWindowController();

signals:
};

#endif // SETTINGSWINDOWCONTROLLER_H
