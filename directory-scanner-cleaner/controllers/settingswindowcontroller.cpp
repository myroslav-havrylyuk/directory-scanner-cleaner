#include "settingswindowcontroller.h"

SettingsWindowController::SettingsWindowController(QObject *parent)
    : QObject{parent} {
    const QUrl url(u"qrc:/directory-scanner-cleaner/views/settingswindow.qml"_qs);
    m_SettingsWindowComponent = new QQmlComponent(gEngine, url);
    m_SettingsQmlContext = new QQmlContext(gEngine->rootContext());
    m_SettingsQmlContext->setContextProperty("SettingsController",
                                             gSettingsController);
    m_SettingsWindow = m_SettingsWindowComponent->create(m_SettingsQmlContext);
    QObject *saveButton = m_SettingsWindow->findChild<QObject *>("save_button");
    QObject::connect(saveButton, SIGNAL(saveSettings()), gSettingsController,
                     SLOT(saveSettings()));
}

SettingsWindowController::~SettingsWindowController() {
    delete m_SettingsWindow;
    delete m_SettingsWindowComponent;
    delete m_SettingsQmlContext;
}
